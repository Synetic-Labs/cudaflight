#!/usr/bin/env python3
"""Force Enzyme to differentiate Betaflight's global-threaded control pipeline.

Reads textual LLVM IR on stdin, writes it on stdout. Three transforms:

1. `enzyme_active` on the pipeline functions. Betaflight threads the control
   signal through module globals across NO-ARGUMENT functions (action -> rcData
   -> rcCommand -> setpointRate -> pidData -> motor; updateRcCommands /
   processRcCommand / pidController / mixTable take no float args). Enzyme's
   interprocedural activity is argument-based, so it marks those calls constant
   and never differentiates them -> the gradient comes out zero. enzyme_active
   forces differentiation (ActivityAnalysis honours it on the callee).
   pt2FilterApply is included because its dterm activity is otherwise inferred
   inconsistently between Enzyme's augmented and gradient sweeps.

2. `enzyme_ta_norecur` on the motor-output globals (@motor, @motor_disarmed).
   THIS IS THE FIX FOR THE TYPEANALYSIS NON-TERMINATION. Those globals are
   `[8 x float]` indexed BOTH by constant byte offsets (motor[0..7]) AND by a
   variable loop index (`gep [8 x float], @motor, 0, %i` in mixTable/stopMotors).
   The variable index injects a `[-1]` (any-offset) wildcard, so TypeAnalysis
   holds the element type in two equivalent-but-distinct encodings -- collapsed
   `[-1,-1]:Float` vs enumerated `[-1,0]:F,[-1,4]:F,...` -- and the
   interprocedural worklist oscillates between them forever (observed: 233k+
   redundant updates, flat RSS, stuck on a partial `{[-1,0]:F,[-1,8]:F}` state).
   norecur makes TA's updateAnalysis a no-op on the global so it never re-derives
   the (already-known `[8 x float]`) type; the pass then converges in ~1s.

3. Gradient-only devirtualization of the setpoint rate curve. processRcCommand
   dispatches the rate curve through a runtime function pointer (@applyRates, set
   from controlRateProfile->rates_type). An INDIRECT active call makes Enzyme
   invert the function POINTER and read a null fn-ptr shadow at runtime ->
   CUDA_ERROR_ILLEGAL_ADDRESS. Devirtualizing it to the concrete curve fixes that
   BUT processRcCommand is SHARED with the firmware's hot loop (taskMainPidLoop),
   where a global devirt lets applyActualRates inline and miscompiles into a wild
   read. So instead we CLONE processRcCommand -> processRcCommand_grad with the
   indirect call devirtualized, and point ONLY bflRateCore (the gradient entry's
   pipeline, called solely by fwCore) at the clone. The firmware keeps the
   original indirect call untouched. Target must match the gym's rates_type
   (default RATES_TYPE_ACTUAL); the FD oracle (real indirect call) validates it.

Pair with register_shadow_globals.py (threads the adjoint through shared shadow
globals). Pipeline order: freeze_config_globals.py | register_shadow_globals.py
rcData rcCommand pidData motor setpointRate | mark_active_pipeline.py
"""

import re
import sys

# enzyme_active: the no-arg pipeline functions + the cloned rate/pid paths. NOTE
# the CLONES (processRcCommand_grad, pidController_grad) are marked, not the
# firmware-shared originals (see CLONES below).
ACTIVE_FNS = ["updateRcCommands", "processRcCommand_grad", "pidController_grad",
              "mixTable", "pt1FilterApply", "pt2FilterApply", "pt3FilterApply",
              # Accessors that return an active global by value across a call
              # boundary; without forcing them active, Enzyme treats the call as
              # constant and the adjoint never reaches the global. getSetpointRate
              # returns setpointRate[axis] -> the setpoint into pidController (the
              # P-term path); without it d@setpointRate is never written and the
              # whole chain upstream of pidController is zero. The others
              # complete the remaining action->motor paths: throttle
              # (mixerGetRcThrottle -> @rcThrottle), feedforward
              # (getFeedforward -> @feedforwardRaw/Smoothed), and stick
              # deflection (getRcDeflection[Abs] -> @rcDeflection*).
              "getSetpointRate", "getFeedforward",
              "getRcDeflection", "getRcDeflectionAbs", "mixerGetRcThrottle"]
NORECUR_GLOBALS = ["motor", "motor_disarmed"]

CLONE_CALLER = "bflRateCore"  # gradient entry's pipeline; redirect its calls to the clones

# Functions SHARED between the firmware hot loop and the gradient pipeline that we
# clone (gradient-only) so we can devirtualize an indirect call inside the clone
# without miscompiling the firmware path. Each clone redirects ONE or more of the
# caller's calls; `devirt` lists (load-regex-with-%capture, concrete target) pairs.
# `expect` is the exact devirt-substitution count — a guard against the IR layout
# drifting out from under a hardcoded match (fail the build loudly instead of
# silently losing the gradient).
CLONES = [
    # processRcCommand dispatches the rate curve through @applyRates (a runtime
    # fn ptr). An indirect ACTIVE call makes Enzyme invert the pointer and read a
    # null shadow -> illegal address. Devirtualize to the concrete curve (default
    # RATES_TYPE_ACTUAL). The FD oracle (real indirect call) validates the target.
    {"fn": "processRcCommand", "clone": "processRcCommand_grad",
     "devirt": [(r'(%\d+) = load ptr, ptr @applyRates\b', "applyActualRates")],
     "expect": 1},
    # pidController runs the yaw P-term through ptermYawLowpassApplyFn (pid.c) —
    # a fn ptr loaded from the (enzyme-inactive) pidRuntime struct, so Enzyme
    # can't devirtualize it and drops the lowpassed-P contribution (~8% of the
    # within-step yaw response). Devirtualize to pt1FilterApply (the configured
    # filter at the default yaw_lowpass_hz=100; 0 would make it nullFilterApply).
    # i64 240 is the byte offset of ptermYawLowpassApplyFn within pidRuntime_s
    # (the 4th filterApplyFnPtr: dtermNotch@24, dtermLowpass@96, dtermLowpass2@168,
    # ptermYawLowpass@240). The FD oracle validates; `expect` guards the offset.
    {"fn": "pidController", "clone": "pidController_grad",
     "devirt": [(r'(%\d+) = load ptr, ptr getelementptr inbounds nuw '
                 r'\(i8, ptr @pidRuntime, i64 240\)', "pt1FilterApply")],
     "expect": 1},
]


def fn_block(lines, name):
    """Return (start, end) line indices of `define ... @name(...) { ... }`."""
    hdr = re.compile(r'^define .*@' + re.escape(name) + r'\(')
    for i, l in enumerate(lines):
        if hdr.match(l):
            for j in range(i + 1, len(lines)):
                if lines[j] == '}':
                    return i, j
    return None, None


def clone_and_devirt(lines, spec):
    """Clone spec['fn'] -> spec['clone'], devirtualizing the indirect calls named
    by spec['devirt'] inside the clone only. Returns (lines, cloned, n_devirt)."""
    s, e = fn_block(lines, spec["fn"])
    if s is None:
        return lines, False, 0
    block = lines[s:e + 1]
    block[0] = block[0].replace('@' + spec["fn"] + '(', '@' + spec["clone"] + '(', 1)
    clone = '\n'.join(block)
    n_devirt = 0
    # devirtualize: for each `%v = load ptr, <ptr-expr>`, rewrite `call ... %v(`
    # (the indirect call through that loaded fn ptr) to a direct call to target.
    for load_re, target in spec["devirt"]:
        for v in re.findall(load_re, clone):
            clone, c = re.subn(r'(call[^\n]*?float )' + re.escape(v) + r'\(',
                               r'\1@' + target + '(', clone)
            n_devirt += c
    # insert the clone right after the original block (as individual lines so the
    # enzyme_active pass below can match its define header)
    lines = lines[:e + 1] + [''] + clone.split('\n') + lines[e + 1:]
    return lines, True, n_devirt


def redirect_calls(lines, caller, orig, clone):
    """Within `caller`, point every `@orig(` call at `@clone(` (the firmware's
    identical calls elsewhere stay untouched). Returns (lines, n_redirect)."""
    cs, ce = fn_block(lines, caller)
    n = 0
    if cs is not None:
        for i in range(cs, ce + 1):
            new = lines[i].replace('@' + orig + '(', '@' + clone + '(')
            if new != lines[i]:
                lines[i] = new
                n += 1
    return lines, n


def main():
    src = sys.stdin.read()
    lines = src.split('\n')

    # 3. Gradient-only clones: clone each shared fn, devirtualize its indirect
    #    call(s) in the clone, and redirect bflRateCore (the gradient entry's
    #    pipeline) at the clones. The firmware hot loop keeps the originals.
    clone_report = {}
    for spec in CLONES:
        lines, cloned, n_devirt = clone_and_devirt(lines, spec)
        lines, n_redirect = redirect_calls(lines, CLONE_CALLER, spec["fn"], spec["clone"])
        clone_report[spec["clone"]] = (cloned, n_devirt, n_redirect)
        exp = spec.get("expect")
        if cloned and exp is not None and n_devirt != exp:
            sys.stderr.write(
                f"   ERROR: {spec['fn']} devirt matched {n_devirt} call(s), "
                f"expected {exp} — the hardcoded IR pattern has drifted "
                f"(check the {spec['fn']} indirect-call site / struct offset)\n")
            sys.exit(1)

    # 1. enzyme_active — insert the attr string before the trailing `#N {` of the
    #    function's define line (handles params with `)` e.g. nofpclass(nan inf)).
    n_active = {}
    for i, l in enumerate(lines):
        m = re.match(r'(define .*@([A-Za-z0-9_]+)\(.*\))'
                     r'( local_unnamed_addr)?( #\d+ \{)$', l)
        if m and m.group(2) in ACTIVE_FNS and 'enzyme_active' not in l:
            lines[i] = m.group(1) + (m.group(3) or '') + ' "enzyme_active"' + m.group(4)
            n_active[m.group(2)] = n_active.get(m.group(2), 0) + 1
    src = '\n'.join(lines)

    # 2. enzyme_ta_norecur on the motor globals.
    ids = [int(x) for x in re.findall(r'^!(\d+) =', src, re.M)]
    mdid = (max(ids) + 1) if ids else 0
    new_md = []
    n_nr = 0
    for g in NORECUR_GLOBALS:
        m = re.search(r'^(@%s = [^\n]*?)$' % re.escape(g), src, re.M)
        if not m or 'enzyme_ta_norecur' in m.group(1):
            continue
        md = mdid + len(new_md)
        src = src[:m.start(1)] + m.group(1) + f', !enzyme_ta_norecur !{md}' + src[m.end(1):]
        new_md.append(f'!{md} = !{{}}')
        n_nr += 1
    if new_md:
        src = src.rstrip('\n') + '\n' + '\n'.join(new_md) + '\n'

    sys.stdout.write(src)
    for clone, (cloned, n_devirt, n_redirect) in clone_report.items():
        sys.stderr.write(
            f'   cloned ->{clone} ({cloned}, devirt {n_devirt}, '
            f'redirected {n_redirect} {CLONE_CALLER} call)\n')
    sys.stderr.write(
        f'   enzyme_active: {n_active}; enzyme_ta_norecur on {n_nr} motor globals\n')
    missing = [f for f in ACTIVE_FNS if f not in n_active]
    if missing:
        sys.stderr.write(f'   WARNING: active fns not found: {missing}\n')


if __name__ == '__main__':
    main()
