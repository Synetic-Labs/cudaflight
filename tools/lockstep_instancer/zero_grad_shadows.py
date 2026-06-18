#!/usr/bin/env python3
"""Zero the Enzyme shadow globals at the start of each bfFwStepGrad invocation.

Reads textual LLVM IR on stdin, writes it on stdout. Runs AFTER Enzyme (so the
__enzyme_autodiff call is expanded into diffefwCore) and BEFORE the instancer.

The @*_enzshadow globals (created by register_shadow_globals.py) are module-level
mutable globals that Enzyme uses to thread the reverse-mode adjoint across the
separately-differentiated pipeline functions. Enzyme ACCUMULATES into them
(atomicrmw fadd) and does not fully re-zero them, so a second bfFwStepGrad launch
sees stale adjoint residue and the gradient drifts (verified: same seed ->
different result each call). They happen to start at zeroinitializer, so the
FIRST launch is clean; subsequent launches are not.

Fix: emit `store <ty> zeroinitializer, ptr @X_enzshadow` for every shadow global
just before the diffefwCore call in bfFwStepGrad. That call sits in the valid-
thread path (after the `k >= __bf_inst_count` bounds guard), so out-of-range
threads -- whose instancer-rebased shadow address would be out of the instance
buffer -- never execute the stores. After the instancer rebases @X_enzshadow to
base + k*stride, this zeros exactly this thread's instance shadows.
"""

import re
import sys


def main():
    src = sys.stdin.read()
    lines = src.split('\n')

    # Collect shadow globals: name, type, align.
    shadows = []
    for l in lines:
        m = re.match(r'^@([A-Za-z0-9_.$]+_enzshadow) = internal global '
                     r'(.+?) zeroinitializer(?:, align (\d+))?\s*$', l)
        if m:
            shadows.append((m.group(1), m.group(2), m.group(3) or '4'))

    if not shadows:
        sys.stdout.write(src)
        sys.stderr.write('   zero_grad_shadows: no shadow globals found\n')
        return

    # Find the diffefwCore call inside bfFwStepGrad and insert zeroing before it.
    out = []
    inserted = False
    in_grad = False
    for l in lines:
        if re.match(r'^define .*@bfFwStepGrad\(', l):
            in_grad = True
        if in_grad and not inserted and re.search(r'call void @diffefwCore\(', l):
            indent = l[:len(l) - len(l.lstrip())]
            for name, ty, align in shadows:
                out.append(f'{indent}store {ty} zeroinitializer, ptr @{name}, align {align}')
            inserted = True
        if in_grad and l == '}':
            in_grad = False
        out.append(l)

    sys.stdout.write('\n'.join(out))
    sys.stderr.write(f'   zero_grad_shadows: zeroed {len(shadows)} shadow globals '
                     f'in bfFwStepGrad (inserted={inserted})\n')


if __name__ == '__main__':
    main()
