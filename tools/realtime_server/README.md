# Realtime server

A single-instance, real-time counterpart to the GPU fleet sim
(`tools/lockstep_instancer`); nothing here is linked into the GPU path.

An external client — typically a game engine or renderer that draws the drone
and its world — connects over localhost TCP; the flight itself runs out of
process in `betaflight_realtime_server.elf`, built from this folder: one real
Betaflight firmware instance stepped in lockstep with a first-principles rigid
body at 1 kHz. Each frame the client sends RC sticks + external impulses
(collisions, explosions, wind) and gets back the drone pose and, on demand,
the OSD character grid.

## Files

- `realtime_server.c` — server main: boot/settle/arm, TCP request/response
  loop, firmware+physics lockstep. The wire protocol is documented in its
  header comment.
- `firstprinciples_physics.c/.h` — C port of drone_models'
  `first_principles` model (what crazyflow integrates), so this path matches
  the training sim's physics. Airframe scalars are a 5" placeholder pending
  sysid.
- `build.sh` — builds the server by reusing the exact firmware compile+link
  of the SITL_LOCKSTEP harness (LTO + sitl.ld), swapping in our main.

## Build & run

```sh
tools/realtime_server/build.sh
obj/main/betaflight_realtime_server.elf --port 5556 --eeprom <tune.eeprom>
```

## Design notes

- Single drone, single instance: the IR instancer is not involved
  (`bflInstanceTemplateFixup` is a no-op in this build). At num_envs=1 this
  CPU path is far faster than real time, unlike the GPU path which amortises
  kernel launches over thousands of drones.
- The firmware runs its own attitude estimator (USE_IMU_CALC), but every step
  the server pins the estimated quaternion to the physics ground truth so the
  OSD horizon can never drift from the rendered pose.
- The physics ground-floor clamp is only active during boot/arm; after that
  the client world's terrain is the ground and collisions arrive as impulses.
