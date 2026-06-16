#!/usr/bin/env python3
# Freeze Betaflight parameter-group (PG) config globals before the Enzyme
# autodiff pass. Reads textual LLVM IR on stdin, writes it on stdout.
#
# Each PG declares three module globals by convention:
#   <name>_System    the live, build-time-initialised config copy
#   <name>_Copy      its backup copy
#   <name>_Registry  constant PG metadata (holds function pointers)
# All three are configuration that is baked at build time and held constant
# across the differentiated within-step (action -> motors) gradient. We stamp
# each with two pieces of metadata Enzyme already honours:
#
#   !enzyme_inactive    -> treat as constant w.r.t. the derivative: no shadow
#                          global is created and reads contribute zero. Without
#                          this Enzyme tries to build shadow globals for them.
#   !enzyme_ta_norecur  -> do NOT let TypeAnalysis refine the global's pointee
#                          type from its byte-offset uses. The config structs
#                          contain unions (e.g. accelerometerConfig_s), whose
#                          overlapping types make TA's fixpoint oscillate and
#                          effectively never terminate (accelerometerConfig_System
#                          alone accounted for ~40% of an overnight, unfinished
#                          run). Freezing the type collapses that to <1s.
#
# This is the build-time half of the "state frozen, truncated through-time
# gradient" design (the runtime state globals are frozen in device_diff.c).

import re
import sys

GLOBAL_RE = re.compile(
    r'^(@[A-Za-z0-9_.$]+(?:_System|_Copy|_Registry) = .*?)(\s*)$')

def main():
    src = sys.stdin.read()
    # Pick a fresh metadata id that does not collide with existing ones.
    ids = [int(x) for x in re.findall(r'^!(\d+) =', src, re.M)]
    mdid = (max(ids) + 1) if ids else 0

    n = 0
    out_lines = []
    for line in src.split('\n'):
        m = GLOBAL_RE.match(line)
        if m and '!enzyme_ta_norecur' not in line:
            n += 1
            line = f'{m.group(1)}, !enzyme_inactive !{mdid}, !enzyme_ta_norecur !{mdid}'
        out_lines.append(line)

    src = '\n'.join(out_lines).rstrip('\n')
    src += f'\n!{mdid} = !{{}}\n'
    sys.stdout.write(src)
    sys.stderr.write(f'   froze {n} PG config globals\n')

if __name__ == '__main__':
    main()
