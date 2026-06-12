"""Oracle tests for the Python BetaflightEnv, mirroring gpu_runner
--test-step but driven entirely through zero-copy torch tensors:

  A  determinism: a fixed action sequence replays bit-exactly across reset
     (motor-trace hashes AND obs identical)
  B  closed-loop hover: a torch P-controller holds 5m through the
     obs/action tensors alone — every torch op runs on the GPU
  C  crash + auto-reset: throttle cut raises dones, auto-reset restores

Run: python test_bfgym.py [num_envs]
"""

import sys
import time

import torch

from bfgym import BetaflightEnv


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 256
    print(f"[bfgym-test] creating {n} instances (boot + settle + arm + snapshot)...")
    t0 = time.time()
    env = BetaflightEnv(n, auto_reset=False)
    print(f"[bfgym-test] created in {time.time() - t0:.1f}s: "
          f"obs {tuple(env.obs.shape)}, actions {tuple(env.actions.shape)}, "
          f"device {env.obs.device}")
    ok = True

    actions = torch.zeros(n, env.act_dim, device=env.device)

    # A: determinism across reset, through the tensor interface
    def burst():
        env.reset()
        for i in range(200):
            actions[:, 2] = 0.36 + 0.1 * torch.sin(torch.tensor(0.1 * i))
            env.step(actions)
        return env.hashes(), env.obs.clone()

    h1, o1 = burst()
    h2, o2 = burst()
    a_ok = h1 == h2 and torch.equal(o1, o2)
    ok = ok and a_ok
    print(f"[bfgym-test] A determinism-across-reset: {'bit-exact' if a_ok else 'FAILED'}")

    # B: torch P-controller hover at 5m — closed loop, all on-device
    obs = env.reset()
    actions.zero_()
    actions[:, 2] = 0.6  # initial climb
    t0 = time.time()
    for _ in range(1000):  # 10s sim at decimation 10
        obs, rew, done = env.step(actions)
        alt = -obs[:, 2]
        vz_up = -obs[:, 5]
        actions[:, 2] = (0.36 + 0.22 * (5.0 - alt) - 0.12 * vz_up).clamp(-1, 1)
    wall = time.time() - t0
    alt = -obs[:, 2]
    any_done = bool(done.any().item())
    in_band = bool(((alt > 3.5) & (alt < 6.5)).all().item())
    mean_alt = float(alt.mean().item())
    b_ok = not any_done and in_band
    ok = ok and b_ok
    print(f"[bfgym-test] B torch-P-controller hover: mean alt {mean_alt:.2f}m "
          f"after 10s, dones={int(any_done)}, in-band={int(in_band)} "
          f"({1000 * n / wall:,.0f} env-steps/s)")

    # C: throttle cut -> crash -> dones -> auto-reset restores
    env.auto_reset = True
    actions.zero_()
    actions[:, 2] = -1.0
    crashed = False
    for i in range(300):
        obs, rew, done = env.step(actions)
        if bool(done.all().item()):
            crashed = True
            break
    print(f"[bfgym-test] C crash: all done after {i + 1} steps: {'yes' if crashed else 'NO'}")
    ok = ok and crashed
    # auto-reset already restored them; next step must be grounded, not done
    actions[:, 2] = 0.36
    obs, rew, done = env.step(actions)
    grounded = bool(((-obs[:, 2]) < 0.5).all().item())
    c_ok = grounded and not bool(done.any().item())
    ok = ok and c_ok
    print(f"[bfgym-test] C auto-reset restore: grounded={int(grounded)} "
          f"dones={int(done.any().item())}")

    print(f"[bfgym-test] {'PASS' if ok else 'FAIL'}")
    env.close()
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
