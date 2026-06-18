#!/usr/bin/env python3
"""Register Enzyme shadow globals for the firmware's active control-state chain.

Reads textual LLVM IR on stdin, writes it on stdout. Global names to shadow are
given as argv.

Betaflight threads the control signal through module globals across function
boundaries: action -> rcData -> rcCommand -> setpointRate -> pidData -> motor.
In reverse mode Enzyme, by default, replaces a non-pointer global with a
*per-function local alloca* shadow (GradientUtils.cpp ~5574). That breaks the
adjoint across the separately-differentiated pipeline functions
(updateRcCommands / processRcCommand / pidController / mixTable / bflGetMotorsRaw):
each gets its own throwaway shadow, so the motor cotangent never propagates back
to the action and the gradient comes out zero.

Attaching `!enzyme_shadow` metadata that points at a real, module-level shadow
global makes Enzyme thread the adjoint through that single shared global instead
(GradientUtils.cpp ~5749), and keeps the global active (ActivityAnalysis.cpp
~1272). The shadow globals are plain mutable state, so the instancer packs them
per-instance just like the primal globals.

Only zeroinitializer globals are handled (all the state globals are); anything
else is left untouched and reported.
"""

import re
import sys


def main():
    names = sys.argv[1:]
    src = sys.stdin.read()
    ids = [int(x) for x in re.findall(r'^!(\d+) =', src, re.M)]
    mdid = (max(ids) + 1) if ids else 0

    new_globals = []
    new_md = []
    done = []
    lines = src.split('\n')
    out = []
    for line in lines:
        # Match a mutable global with a zero initializer: either the
        # `zeroinitializer` keyword (aggregates) or a scalar zero (`float
        # 0.000000e+00`, `0`, `null`). The shadow is always a same-typed
        # zeroinitializer, so the exact initializer text is irrelevant.
        m = re.match(r'^(@([A-Za-z0-9_.$]+)) = (.*\bglobal )(.+?) '
                     r'(?:zeroinitializer|0\.0+e\+00|0|null)'
                     r'(, align \d+)?\s*$', line)
        if m and m.group(2) in names and '!enzyme_shadow' not in line:
            name, ty, align = m.group(2), m.group(4), m.group(5) or ''
            md = mdid + len(done)
            shadow = f'@{name}_enzshadow'
            # Internal mutable shadow, same type/align; instancer packs it.
            new_globals.append(
                f'{shadow} = internal global {ty} zeroinitializer{align}')
            new_md.append(f'!{md} = !{{ptr {shadow}}}')
            line = f'{line.rstrip()}, !enzyme_shadow !{md}'
            done.append(name)
        out.append(line)

    # Globals + their metadata are module-level; append where they are legal.
    # New global definitions go right after the last existing global def; the
    # metadata nodes go at end of file. Simplest correct placement: globals just
    # before the first `define`, metadata at EOF.
    text = '\n'.join(out)
    if new_globals:
        gi = text.find('\ndefine ')
        block = '\n' + '\n'.join(new_globals) + '\n'
        text = text[:gi] + block + text[gi:] if gi != -1 else text + block
        text = text.rstrip('\n') + '\n' + '\n'.join(new_md) + '\n'

    sys.stdout.write(text)
    missing = sorted(set(names) - set(done))
    sys.stderr.write(f'   registered {len(done)} shadow globals: {done}\n')
    if missing:
        sys.stderr.write(f'   WARNING: not found / not zeroinit: {missing}\n')


if __name__ == '__main__':
    main()
