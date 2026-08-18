# SPDX-License-Identifier: GPL-3.0-or-later
"""Oracle tests for BetaflightJaxEnv — same suite as test_torch_env.py,
driven through JAX arrays:

  A  determinism: fixed action sequence replays bit-exactly across reset
  B  closed-loop hover: a jnp P-controller holds 5m through step() alone
  C  crash + auto-reset: throttle cut raises dones, auto-reset restores

Run: python test_jax_env.py [num_envs]
"""

import sys
import time

# importing jnp through the env module keeps the XLA preallocation
# env var set before jax first touches the GPU
from cudaflight.jax_env import BetaflightJaxEnv, jnp

import jax


def main() -> None:
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 256
    print(f"[cudaflight-jax] creating {n} instances (boot + settle + arm + snapshot)...")
    t0 = time.time()
    env = BetaflightJaxEnv(n, auto_reset=False)
    print(f"[cudaflight-jax] created in {time.time() - t0:.1f}s: "
          f"jax {jax.__version__}, device {env.device}")
    ok = True

    zeros = jnp.zeros((n, env.act_dim), jnp.float32)
    throttle = lambda v: zeros.at[:, 2].set(v)

    # A: determinism across reset, through the JAX interface
    def burst():
        env.reset()
        obs = None
        for i in range(200):
            obs, _, _ = env.step(throttle(0.36 + 0.1 * jnp.sin(0.1 * i)))
        return env.hashes(), obs

    h1, o1 = burst()
    h2, o2 = burst()
    a_ok = h1 == h2 and bool(jnp.array_equal(o1, o2))
    ok = ok and a_ok
    print(f"[cudaflight-jax] A determinism-across-reset: {'bit-exact' if a_ok else 'FAILED'}")

    # B: jnp P-controller hover at 5m — closed loop, all on-device
    env.reset()
    actions = throttle(0.6)  # initial climb
    t0 = time.time()
    for _ in range(1000):  # 10s sim at decimation 10
        obs, _, done = env.step(actions)
        alt = -obs[:, 2]
        vz_up = -obs[:, 5]
        actions = zeros.at[:, 2].set(jnp.clip(0.36 + 0.22 * (5.0 - alt) - 0.12 * vz_up, -1, 1))
    wall = time.time() - t0
    alt = -obs[:, 2]
    any_done = bool(done.any())
    in_band = bool(((alt > 3.5) & (alt < 6.5)).all())
    mean_alt = float(alt.mean())
    b_ok = not any_done and in_band
    ok = ok and b_ok
    print(f"[cudaflight-jax] B jnp-P-controller hover: mean alt {mean_alt:.2f}m "
          f"after 10s, dones={int(any_done)}, in-band={int(in_band)} "
          f"({1000 * n / wall:,.0f} env-steps/s)")

    # C: throttle cut -> crash -> dones -> auto-reset restores
    env.auto_reset = True
    actions = throttle(-1.0)
    crashed = False
    for i in range(300):
        obs, _, done = env.step(actions)
        if bool(done.all()):
            crashed = True
            break
    print(f"[cudaflight-jax] C crash: all done after {i + 1} steps: {'yes' if crashed else 'NO'}")
    ok = ok and crashed
    obs, _, done = env.step(throttle(0.36))
    grounded = bool(((-obs[:, 2]) < 0.5).all())
    c_ok = grounded and not bool(done.any())
    ok = ok and c_ok
    print(f"[cudaflight-jax] C auto-reset restore: grounded={int(grounded)} "
          f"dones={int(done.any())}")

    print(f"[cudaflight-jax] {'PASS' if ok else 'FAIL'}")
    env.close()
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
