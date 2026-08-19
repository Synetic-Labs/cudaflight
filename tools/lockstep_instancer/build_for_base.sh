#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Build the cudaflight wheel for an arbitrary Betaflight base commit.
#
#   tools/lockstep_instancer/build_for_base.sh <commit-ish> [--cpu-only] [--release]
#
# The repo stays version-free: a firmware version is a PARAMETER. This
# script materializes one build: a detached worktree at the base commit,
# the fork delta applied on top, the wheel versioned 0.5.0+bf.<short-hash>
# so the base identity rides in the wheel itself (the render's version
# gate reads it back). --release uploads the wheel as a GitHub release
# asset tagged cudaflight-v<version>; without it the wheel stays local.
# --cpu-only builds just libcpuflight.so (enough to gate/render a dump,
# e.g. while bisecting a 'norevision' dump to its base).
#
# The fork delta is diff(<nearest tag on master>, master): additions are
# checked out from master; modifications apply as patches (a context
# mismatch on an old base fails loudly for a manual port); upstream file
# deletions (CI workflows) are dropped. Requires: LLVM 20 at
# /usr/lib/llvm-20, CUDA at /usr/local/cuda (GPU build), and the build
# venv at tools/lockstep_instancer/python/.venv of the MAIN checkout
# (jax pinned per python/Makefile policy).

set -euo pipefail
cd "$(dirname "$0")/../.."
MAIN=$PWD

BASE_REF=${1:?usage: build_for_base.sh <commit-ish> [--cpu-only] [--release]}
CPU_ONLY=false
RELEASE=false
for arg in "${@:2}"; do
    case "$arg" in
        --cpu-only) CPU_ONLY=true ;;
        --release)  RELEASE=true ;;
        *) echo "unknown flag: $arg" >&2; exit 1 ;;
    esac
done

BASE=$(git rev-parse --verify "$BASE_REF^{commit}")
SHORT=$(git rev-parse --short=8 "$BASE")
REV9=$(git rev-parse --short=9 "$BASE")
BASEVER=0.5.0
VERSION="$BASEVER+bf.$SHORT"
# nearest BETAFLIGHT tag under master = the base the fork delta is diffed
# against; cudaflight release tags (cudaflight-v*) are not tree anchors
FORK_BASE=$(git describe --tags --abbrev=0 --exclude 'cudaflight-*' master)
WT=${CUDAFLIGHT_WORKTREES:-$HOME/.cache/cudaflight/worktrees}/bf-$SHORT

echo "== base $BASE ($(git log -1 --format='%ad %s' --date=short "$BASE" | head -c 70))"
echo "== fork delta: $FORK_BASE..master -> wheel $VERSION"

# fresh worktree at the base
if [ -d "$WT" ]; then
    git worktree remove --force "$WT"
fi
mkdir -p "$(dirname "$WT")"
git worktree add --quiet --detach "$WT" "$BASE"

# apply the fork delta (worktrees share the object db, so 'master' resolves)
cd "$WT"
git diff --name-status --no-renames "$FORK_BASE" master | while IFS=$'\t' read -r st path; do
    case "$st" in
        A) git checkout --quiet master -- "$path" ;;
        M) git diff "$FORK_BASE" master -- "$path" | git apply \
               || { echo "PATCH FAILED on $path — port the fork hunk to this base manually" >&2; exit 1; }
           ;;
        D) rm -f "$path" ;;
        *) echo "unhandled delta status '$st' for $path" >&2; exit 1 ;;
    esac
done

# bake the wheel identity
sed -i "s|^VERSION    ?= .*|VERSION    ?= $VERSION|" tools/lockstep_instancer/python/Makefile
sed -i "s|^version = \".*\"|version = \"$VERSION\"|" tools/lockstep_instancer/python/pyproject.toml

export PATH=/usr/local/cuda/bin:/usr/lib/llvm-20/bin:$PATH
export CUDA_HOME=/usr/local/cuda
export BFL_REVISION=$REV9   # the firmware header reports the BASE, not the dirty worktree

echo "== building libcpuflight.so"
bash tools/lockstep_instancer/build_multi.sh > "$WT/build_multi.log" 2>&1 \
    || { tail -20 "$WT/build_multi.log" >&2; exit 1; }

echo "== self-test (stock dump renders strictly on its own firmware)"
obj/multi/betaflight_SITL_LOCKSTEP_MULTI --dump-cli obj/multi/stock_dump.txt > /dev/null
grep -m1 "# Betaflight /" obj/multi/stock_dump.txt
PYTHONPATH="$WT/tools/lockstep_instancer/python/src" \
CPUFLIGHT_LIB="$WT/obj/multi/libcpuflight.so" \
"$MAIN/tools/lockstep_instancer/python/.venv/bin/python" \
    -m cudaflight.config obj/multi/stock_dump.txt

if $CPU_ONLY; then
    echo "== done (cpu-only): $WT/obj/multi/libcpuflight.so"
    exit 0
fi

echo "== building wheel $VERSION"
TARGET=wheel
if $RELEASE; then TARGET=release; fi
make -C tools/lockstep_instancer/python "$TARGET" \
    PYTHON="$MAIN/tools/lockstep_instancer/python/.venv/bin/python" \
    > "$WT/build_wheel.log" 2>&1 \
    || { tail -20 "$WT/build_wheel.log" >&2; exit 1; }

ls tools/lockstep_instancer/python/dist/*.whl
echo "== done"
