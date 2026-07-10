/*
 * First-principles quad dynamics — a C port of drone_models' first_principles
 * model (the closed-form ODE crazyflow integrates), so the realtime CPU SITL
 * path matches the training sim's physics instead of the old hand-rolled quadSim.
 *
 * Structure (motor permutation, mixing matrix, rotor lag, FLU<->FRD flips,
 * sensor synthesis, explicit-Euler integration) is ported faithfully from the
 * validated reference. Only the scalar airframe params are a reasonable 5"
 * placeholder for now (see FP_* defines in the .c) — replace with sysid values.
 *
 * Pure C, no firmware/MuJoCo deps, so it is unit-testable standalone. The SITL
 * server does the firmware coupling around it (bflGetMotorsNormalised ->
 * fpStep -> fpGetImu -> bflSetGyroAccel/bflSetBaro).
 *
 * Internal state is in the model's native frame (world Z-up, body FLU, quat
 * xyzw, rotor speeds in RPM). Outputs are converted to Betaflight
 * conventions (body FRD gyro/accel, NED pose, wxyz quat).
 */
#ifndef FIRSTPRINCIPLES_PHYSICS_H
#define FIRSTPRINCIPLES_PHYSICS_H

#include <stdbool.h>
#include <stdint.h>

// Enable/disable the ground floor clamp (default on). The realtime GTA server
// turns it off after arming so the drone can descend into world terrain instead
// of being pinned to its spawn altitude — in-game collision handles the ground.
void fpSetFloorEnabled(bool enabled);

typedef struct {
    double pos[3];      // world, Z-up, m
    double quat[4];     // body->world, xyzw, FLU
    double vel[3];      // world, m/s
    double omega[3];    // body angular rate, rad/s (FLU)
    double rotor[4];    // rotor speed, RPM (crazyflow rotor order M1..M4)
} fpSim_t;

// Initialise to rest on the ground at the origin, level, rotors stopped.
void fpInit(fpSim_t *s);

// Respawn to a clean, level hover `altitude` metres above the origin (zero
// velocity/rates, rotors at hover). Used by the reset feature.
void fpReset(fpSim_t *s, double altitude);

// Advance the dynamics by dt seconds given the firmware's 4 normalised motor
// outputs [0,1] in Betaflight order [REAR_R, FRONT_R, REAR_L, FRONT_L].
void fpStep(fpSim_t *s, const float motorsBf[4], double dt);

// Apply a one-shot external kick: linear impulse (m/s, NED world) and angular
// impulse (rad/s, body FRD) — the external-disturbance channel (collisions /
// explosions / wind).
void fpImpulse(fpSim_t *s, const float impLinNed[3], const float impAngFrd[3]);

// Synthesised IMU for the firmware: gyro (rad/s) + accel specific force (m/s^2)
// in body FRD, and barometric pressure (Pa). No firmware calls — the caller
// feeds these to bflSetGyroAccel / bflSetBaro.
void fpGetImu(const fpSim_t *s, double gyroFrd[3], double accelFrd[3], int32_t *baroPa);

// Pose for the wire protocol: position (NED, m) and body->world quaternion
// (wxyz) in the NED/FRD convention.
void fpGetPoseNed(const fpSim_t *s, float posNed[3], float quatNedWxyz[4]);

// Euler angles (deg) in the Betaflight convention (roll right+, pitch nose-up+,
// yaw right+), for debug/telemetry.
void fpEulerDeg(const fpSim_t *s, double *roll, double *pitch, double *yaw);

#endif
