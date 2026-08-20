# SPDX-License-Identifier: GPL-3.0-or-later
"""The Wall of Betaflights.

N real Betaflight firmware instances run on the GPU, each drawing its
own OSD (osd.c / osd_elements.c through the fb-osd displayport) into a
per-instance character grid. This demo tiles those grids — rendered
with the authentic MAX7456 font, composited entirely on-device — into
one window: a wall of live OSDs, every artificial horizon, armed timer
and craft name computed by real flight firmware.

The fleet flies the in-kernel physics with a torch hover controller plus
per-instance stick wiggles, so every tile tilts differently. Keys:

  c          crash a random third of the fleet (throttle cut; watch them
             fall, disarm, and snap back to the armed snapshot — the
             armed timers visibly reset)
  page up/dn pan the wall across the fleet
  +/-        tile scale
  b          toggle blink rendering
  ESC/q      quit

Run (installed with the [viz] extra):
  wall-of-betaflight 256 --show 6x3
  wall-of-betaflight 64 --headless --frames 120 --screenshot wall.png
"""

import argparse
import math
import os
import sys
import time

import torch

from .torch_env import BetaflightEnv
from .osd_wall import GLYPH_H, GLYPH_W, OsdWall


def _horizon_backdrop(obs: "torch.Tensor", h: int, w: int) -> "torch.Tensor":
    """FPV-style backdrop from each instance's real state, [K, h, w, 3] u8.

    Stands in for the camera feed a real MAX7456 would overlay: sky
    gradient with a sun, and a perspective checkerboard ground. The
    horizon follows the firmware's own quaternion (obs 6:10, wxyz, NED)
    and the checker density follows its altitude, so every tile moves
    with its aircraft."""
    dev = obs.device
    qw, qx, qy, qz = obs[:, 6], obs[:, 7], obs[:, 8], obs[:, 9]
    roll = torch.atan2(2 * (qw * qx + qy * qz), 1 - 2 * (qx * qx + qy * qy))
    pitch = torch.asin((2 * (qw * qy - qz * qx)).clamp(-1.0, 1.0))
    yaw = torch.atan2(2 * (qw * qz + qx * qy), 1 - 2 * (qy * qy + qz * qz))
    alt = (-obs[:, 2]).clamp(1.0, 50.0)[:, None, None]

    y = torch.linspace(-1.0, 1.0, h, device=dev)[None, :, None]
    x = torch.linspace(-w / h, w / h, w, device=dev)[None, None, :]
    cr = torch.cos(roll)[:, None, None]
    sr = torch.sin(roll)[:, None, None]
    d = cr * (y - 1.8 * pitch[:, None, None]) - sr * x  # >0 below horizon
    p = sr * y + cr * x                                 # along the horizon

    def col(r: float, g: float, b: float) -> "torch.Tensor":
        return torch.tensor([r, g, b], device=dev)

    # sky: zenith->horizon gradient plus a small sun placed by the
    # aircraft's yaw — different headings put it elsewhere, or off-tile
    a = (d.neg().clamp(0.0, 1.2) / 1.2)[..., None]
    sky = col(150., 196., 228.) * (1 - a) + col(38., 92., 166.) * a
    p_sun = (1.7 * torch.sin(yaw + 0.7))[:, None, None]
    sun_r2 = (p - p_sun) ** 2 + (d + 0.42) ** 2
    sky = sky + col(255., 244., 200.) * torch.exp(-sun_r2 / 0.0009)[..., None]
    sky = sky + col(255., 224., 150.) * 0.3 * torch.exp(-sun_r2 / 0.008)[..., None]

    # ground: perspective checkerboard, denser with altitude, hazy at the
    # horizon (u, v blow up there; the haze hides the aliasing)
    dg = d.clamp_min(1e-3)
    u = p / dg
    v = alt / (10.0 * dg)
    checker = ((u * 1.3).floor() + (v * 1.3).floor()).remainder(2.0)[..., None]
    ground = col(88., 124., 66.) * (1 - checker) + col(70., 104., 54.) * checker
    haze = torch.exp(-dg / 0.12)[..., None]
    ground = ground * (1 - haze) + col(168., 190., 204.) * haze

    img = torch.where(d[..., None] < 0.0, sky, ground)
    return img.round().clamp(0, 255).to(torch.uint8)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Wall of Betaflights")
    p.add_argument("envs", nargs="?", type=int, default=64,
                   help="firmware instances on the GPU (default 64)")
    p.add_argument("--show", default="5x3",
                   help="tiles shown as CxR (default 5x3)")
    p.add_argument("--scale", type=float, default=1.0, help="tile scale")
    p.add_argument("--fps", type=int, default=30,
                   help="render rate; also sets the control decimation")
    p.add_argument("--eeprom", default=None,
                   help="boot-ready EEPROM image (e.g. from --cli-dump): the"
                        " wall then shows that quad's own OSD layout")
    p.add_argument("--backdrop", action="store_true",
                   help="draw a sky/ground horizon behind each OSD, driven by"
                        " the instance's attitude (default: black, like a"
                        " real OSD with no camera)")
    p.add_argument("--headless", action="store_true",
                   help="no window (SDL dummy driver); use with --screenshot")
    p.add_argument("--frames", type=int, default=0,
                   help="exit after this many frames (0 = run until quit)")
    p.add_argument("--screenshot", default=None,
                   help="save the last frame as PNG")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    tx, ty = (int(v) for v in args.show.lower().split("x"))
    show = tx * ty

    if args.headless:
        os.environ.setdefault("SDL_VIDEODRIVER", "dummy")
    import pygame

    n = max(args.envs, show)
    print(f"[wall] booting {n} firmware instances on the GPU "
          f"(settle + arm + snapshot)...")
    t0 = time.time()
    env = BetaflightEnv(n, decimation=max(1, round(1000 / args.fps)),
                        eeprom=args.eeprom)
    print(f"[wall] fleet up in {time.time() - t0:.1f}s "
          f"({env.osd_rows}x{env.osd_cols} OSD grids)")

    wall = OsdWall(env.device, rows=env.osd_rows, cols=env.osd_cols)
    win_w = round(tx * wall.tile_w * args.scale)
    win_h = round(ty * wall.tile_h * args.scale)

    pygame.init()
    screen = pygame.display.set_mode((win_w, win_h))
    pygame.display.set_caption(
        f"Wall of Betaflights — {n} real firmware instances on "
        f"{torch.cuda.get_device_name(env.device)}")
    clock = pygame.time.Clock()

    k = torch.arange(n, device=env.device, dtype=torch.float32)
    target_alt = 4.0 + 2.0 * (k % 5) / 4.0
    # golden-ratio spacing decorrelates neighbours: adjacent tiles get
    # unrelated wiggle phases instead of a smooth gradient across the wall
    phase = 2.0 * math.pi * torch.frac(k * 0.6180339887)
    actions = torch.zeros(n, env.act_dim, device=env.device)
    actions[:, 2] = 0.6  # initial climb

    crash_until = -1.0
    crash_mask = torch.zeros(n, dtype=torch.bool, device=env.device)
    offset = 0
    blink = True
    sim_t = 0.0
    frame = 0
    last_img = None

    print("[wall] flying. keys: c crash wave, PgUp/PgDn pan, +/- scale, "
          "b blink, q quit")
    running = True
    while running and (args.frames == 0 or frame < args.frames):
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                running = False
            elif ev.type == pygame.KEYDOWN:
                if ev.key in (pygame.K_ESCAPE, pygame.K_q):
                    running = False
                elif ev.key == pygame.K_c:
                    crash_mask = torch.rand(n, device=env.device) < 0.33
                    crash_until = sim_t + 1.5
                    print(f"[wall] crashing {int(crash_mask.sum())} instances")
                elif ev.key == pygame.K_PAGEDOWN:
                    offset = (offset + show) % n
                elif ev.key == pygame.K_PAGEUP:
                    offset = (offset - show) % n
                elif ev.key == pygame.K_b:
                    blink = not blink
                elif ev.key in (pygame.K_PLUS, pygame.K_EQUALS, pygame.K_MINUS):
                    args.scale *= 1.25 if ev.key != pygame.K_MINUS else 0.8
                    win_w = round(tx * wall.tile_w * args.scale)
                    win_h = round(ty * wall.tile_h * args.scale)
                    screen = pygame.display.set_mode((win_w, win_h))

        # --- policy: hover P-controller + phase-offset stick wiggles
        obs, _, _ = env.step(actions)
        sim_t += env.decimation * 1e-3
        alt = -obs[:, 2]
        vz_up = -obs[:, 5]
        actions[:, 2] = (0.36 + 0.22 * (target_alt - alt) - 0.12 * vz_up)
        t = torch.full_like(phase, sim_t)
        actions[:, 0] = 0.16 * torch.sin(2 * math.pi * 0.40 * t + phase)
        actions[:, 1] = 0.12 * torch.sin(2 * math.pi * 0.27 * t + 1.7 * phase)
        actions[:, 3] = 0.08 * torch.sin(2 * math.pi * 0.15 * t + 2.4 * phase)
        if sim_t < crash_until:
            actions[crash_mask, :] = 0.0
            actions[crash_mask, 2] = -1.0
        actions.clamp_(-1.0, 1.0)

        # --- render: firmware grids -> glyph mosaic, all on the GPU
        env.osd_update()
        sel = (torch.arange(show, device=env.device) + offset) % n
        blink_phase = blink and (frame // max(1, args.fps // 2)) % 2 == 1
        backdrop = (_horizon_backdrop(obs[sel], wall.rows * GLYPH_H,
                                      wall.cols * GLYPH_W)
                    if args.backdrop else None)
        img = wall.frame(env.osd[sel], env.osd_attrs[sel], grid=(ty, tx),
                         blink_phase=blink_phase, scale=args.scale,
                         background=backdrop)
        last_img = img
        arr = img.cpu().numpy()
        surf = pygame.surfarray.make_surface(arr.swapaxes(0, 1))
        screen.blit(surf, (0, 0))
        pygame.display.flip()

        clock.tick(args.fps)
        frame += 1

    if args.screenshot and last_img is not None:
        from PIL import Image
        Image.fromarray(last_img.cpu().numpy()).save(args.screenshot)
        print(f"[wall] screenshot -> {args.screenshot}")

    pygame.quit()
    env.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
