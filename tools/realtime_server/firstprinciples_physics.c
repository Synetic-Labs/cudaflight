/*
 * See firstprinciples_physics.h. C port of drone_models' first_principles ODE
 * (the model crazyflow integrates), explicit-Euler, single drone.
 *
 * The structure is ported from the validated reference and must not be tuned:
 *   - rotor order is crazyflow's M1..M4; firmware motors (Betaflight order
 *     [REAR_R, FRONT_R, REAR_L, FRONT_L]) map in via FP_BF_TO_CF = {1,0,2,3}.
 *   - mixing matrix, body drag, rotor first-order lag, prop gyroscopic term.
 *   - frames: model is world Z-up / body FLU / quat xyzw; Betaflight wants
 *     world NED / body FRD. The single transform for any body OR world vector
 *     is the 180-deg-about-x flip (x,-y,-z); the quaternion is conjugated by
 *     qf=(w=0,x=1,y=0,z=0).
 *   - explicit Euler; the attitude update uses the rotation-vector exponential
 *     of omega*dt, not a linearised quat_dot.
 *
 * The scalars below are a 5" placeholder; sysid later replaces them.
 */
#include "firstprinciples_physics.h"

#include <math.h>
#include <stdbool.h>
#include <string.h>

// Ground floor clamp, on by default so the drone rests on the ground during
// firmware boot/arm instead of free-falling; see fpSetFloorEnabled in the
// header for why the realtime server drops it after arming.
static bool s_floorEnabled = true;

void fpSetFloorEnabled(bool enabled)
{
    s_floorEnabled = enabled;
}

// ---- 5" airframe scalars (placeholder; replace with sysid) -----------------
#define FP_MASS         0.6      // kg
#define FP_G            9.81     // m/s^2
#define FP_ARM_L        0.078    // roll/pitch moment arm, m (X-quad motor offset)
#define FP_JXX          2.5e-3   // body inertia, kg.m^2
#define FP_JYY          2.5e-3
#define FP_JZZ          4.0e-3
#define FP_KF           1.467e-8 // thrust = KF * rpm^2  (N); ~13.2N/motor (1.35 kgf) at 30k rpm
#define FP_KT           2.35e-10 // rotor drag torque = KT * rpm^2 (N.m); KT/KF ~ 0.016
#define FP_DRAGX       -0.30     // body linear drag, N per (m/s)
#define FP_DRAGY       -0.30
#define FP_DRAGZ       -0.35
#define FP_ROTOR_UP     35.0     // rotor spin-up rate, 1/s (tau ~ 29 ms)
#define FP_ROTOR_DOWN   45.0     // rotor spin-down rate, 1/s
#define FP_PROP_INERTIA 1.0e-5   // per-rotor spin inertia, kg.m^2
#define FP_THRUST_MIN   0.10     // idle thrust/motor (N) -> idle rpm
#define FP_THRUST_MAX   13.20    // max thrust/motor (N) -> max rpm; ~1.35 kgf, TWR ~9:1 (powerful 5")
#define FP_BARO_P0      101325.0 // Pa, sea level
#define FP_BARO_SCALE   8434.0   // barometric scale height, m
#define FP_RPM2RADS     (2.0 * M_PI / 60.0)

// crazyflow rotor order; rows = roll, pitch, yaw mixing (validated reference).
static const double MIX[3][4] = {
    {-1.0, -1.0,  1.0,  1.0},
    {-1.0,  1.0,  1.0, -1.0},
    {-1.0,  1.0, -1.0,  1.0},
};
static const int FP_BF_TO_CF[4] = {1, 0, 2, 3};

static double rpmIdle(void) { return sqrt(FP_THRUST_MIN / FP_KF); }
static double rpmMax(void)  { return sqrt(FP_THRUST_MAX / FP_KF); }
static double rpmHover(void) { return sqrt((FP_MASS * FP_G / 4.0) / FP_KF); }

// R(q) * v, q xyzw, body->world
static void rot(const double q[4], const double v[3], double out[3])
{
    const double x = q[0], y = q[1], z = q[2], w = q[3];
    out[0] = (1 - 2 * (y * y + z * z)) * v[0] + 2 * (x * y - w * z) * v[1] + 2 * (x * z + w * y) * v[2];
    out[1] = 2 * (x * y + w * z) * v[0] + (1 - 2 * (x * x + z * z)) * v[1] + 2 * (y * z - w * x) * v[2];
    out[2] = 2 * (x * z - w * y) * v[0] + 2 * (y * z + w * x) * v[1] + (1 - 2 * (x * x + y * y)) * v[2];
}

// R(q)^T * v (world->body)
static void rotInv(const double q[4], const double v[3], double out[3])
{
    const double qc[4] = {-q[0], -q[1], -q[2], q[3]};
    rot(qc, v, out);
}

// Hamilton product a (x) b, xyzw
static void qmul(const double a[4], const double b[4], double o[4])
{
    const double ax = a[0], ay = a[1], az = a[2], aw = a[3];
    const double bx = b[0], by = b[1], bz = b[2], bw = b[3];
    o[0] = aw * bx + ax * bw + ay * bz - az * by;
    o[1] = aw * by - ax * bz + ay * bw + az * bx;
    o[2] = aw * bz + ax * by - ay * bx + az * bw;
    o[3] = aw * bw - ax * bx - ay * by - az * bz;
}

void fpInit(fpSim_t *s)
{
    memset(s, 0, sizeof(*s));
    s->quat[3] = 1.0; // identity
}

void fpReset(fpSim_t *s, double altitude)
{
    memset(s, 0, sizeof(*s));
    s->quat[3] = 1.0;
    s->pos[2] = altitude;                 // Z-up
    const double rh = rpmHover();
    for (int i = 0; i < 4; i++) {
        s->rotor[i] = rh; // start hovering, not falling
    }
}

void fpStep(fpSim_t *s, const float motorsBf[4], double dt)
{
    // motor [0,1] (bf order) -> crazyflow rotor order -> commanded rpm
    const double rIdle = rpmIdle(), rSpan = rpmMax() - rpmIdle();
    double cmd[4], rotorDot[4];
    for (int i = 0; i < 4; i++) {
        const double mc = (double)motorsBf[FP_BF_TO_CF[i]];
        cmd[i] = rIdle + rSpan * (mc < 0 ? 0 : (mc > 1 ? 1 : mc));
        const double d = cmd[i] - s->rotor[i];
        rotorDot[i] = (d > 0 ? FP_ROTOR_UP : FP_ROTOR_DOWN) * d; // first-order lag
    }

    // per-rotor thrust + drag torque from the current rotor state
    double f[4], tq[4], fTotal = 0.0;
    for (int i = 0; i < 4; i++) {
        f[i] = FP_KF * s->rotor[i] * s->rotor[i];
        tq[i] = FP_KT * s->rotor[i] * s->rotor[i];
        fTotal += f[i];
    }

    // forces (world): thrust + gravity + body drag
    const double thrustBody[3] = {0, 0, fTotal};
    double thrustWorld[3];
    rot(s->quat, thrustBody, thrustWorld);
    double velBody[3];
    rotInv(s->quat, s->vel, velBody);
    const double dragBody[3] = {FP_DRAGX * velBody[0], FP_DRAGY * velBody[1], FP_DRAGZ * velBody[2]};
    double dragWorld[3];
    rot(s->quat, dragBody, dragWorld);
    double velDot[3];
    for (int i = 0; i < 3; i++) {
        velDot[i] = (thrustWorld[i] + dragWorld[i]) / FP_MASS;
    }
    velDot[2] += -FP_G; // gravity (Z-up)

    // torques (body)
    double mixF0 = 0, mixF1 = 0, mixT2 = 0, sumRv = 0, sumRvDot = 0;
    for (int i = 0; i < 4; i++) {
        mixF0 += MIX[0][i] * f[i];
        mixF1 += MIX[1][i] * f[i];
        mixT2 += MIX[2][i] * tq[i];
        sumRv += MIX[2][i] * s->rotor[i] * FP_RPM2RADS;
        sumRvDot += MIX[2][i] * rotorDot[i] * FP_RPM2RADS;
    }
    double tau[3];
    tau[0] = FP_ARM_L * mixF0 + FP_PROP_INERTIA * (-s->omega[1] * sumRv);
    tau[1] = FP_ARM_L * mixF1 + FP_PROP_INERTIA * (-s->omega[0] * sumRv);
    tau[2] = mixT2 + FP_PROP_INERTIA * sumRvDot;

    // rigid-body Euler eqn: omega_dot = Jinv (tau - omega x (J omega))
    const double J[3] = {FP_JXX, FP_JYY, FP_JZZ};
    const double Jw[3] = {J[0] * s->omega[0], J[1] * s->omega[1], J[2] * s->omega[2]};
    const double cross[3] = {
        s->omega[1] * Jw[2] - s->omega[2] * Jw[1],
        s->omega[2] * Jw[0] - s->omega[0] * Jw[2],
        s->omega[0] * Jw[1] - s->omega[1] * Jw[0],
    };
    double omegaDot[3];
    for (int i = 0; i < 3; i++) {
        omegaDot[i] = (tau[i] - cross[i]) / J[i];
    }

    // explicit Euler: all updates from the current state
    // attitude via rotation-vector exponential of omega*dt
    const double rv[3] = {s->omega[0] * dt, s->omega[1] * dt, s->omega[2] * dt};
    const double ang = sqrt(rv[0] * rv[0] + rv[1] * rv[1] + rv[2] * rv[2]);
    double dq[4];
    if (ang > 1e-9) {
        const double sh = sin(ang * 0.5) / ang;
        dq[0] = rv[0] * sh; dq[1] = rv[1] * sh; dq[2] = rv[2] * sh; dq[3] = cos(ang * 0.5);
    } else {
        dq[0] = 0; dq[1] = 0; dq[2] = 0; dq[3] = 1;
    }
    double qn[4];
    qmul(s->quat, dq, qn);
    const double qnorm = sqrt(qn[0] * qn[0] + qn[1] * qn[1] + qn[2] * qn[2] + qn[3] * qn[3]);

    for (int i = 0; i < 3; i++) {
        s->pos[i] += s->vel[i] * dt;
        s->vel[i] += velDot[i] * dt;
        s->omega[i] += omegaDot[i] * dt;
    }
    for (int i = 0; i < 4; i++) {
        s->rotor[i] += rotorDot[i] * dt;
        if (s->rotor[i] < 0) s->rotor[i] = 0;
        s->quat[i] = qn[i] / qnorm;
    }

    // floor clamp (matches crazyflow clip_floor_pos): keep above ground, kill
    // linear velocity on contact (attitude/rates untouched). Skipped once the
    // realtime server disables it, so the drone can descend into world terrain.
    if (s_floorEnabled && s->pos[2] < -0.001) {
        s->pos[2] = -0.001;
        s->vel[0] = s->vel[1] = s->vel[2] = 0;
    }
}

void fpImpulse(fpSim_t *s, const float impLinNed[3], const float impAngFrd[3])
{
    // NED world -> Z-up world and FRD body -> FLU body are both (x,-y,-z).
    s->vel[0] += (double)impLinNed[0];
    s->vel[1] += -(double)impLinNed[1];
    s->vel[2] += -(double)impLinNed[2];
    s->omega[0] += (double)impAngFrd[0];
    s->omega[1] += -(double)impAngFrd[1];
    s->omega[2] += -(double)impAngFrd[2];
}

void fpGetImu(const fpSim_t *s, double gyroFrd[3], double accelFrd[3], int32_t *baroPa)
{
    gyroFrd[0] = s->omega[0];
    gyroFrd[1] = -s->omega[1];
    gyroFrd[2] = -s->omega[2];

    double fTotal = 0.0;
    for (int i = 0; i < 4; i++) {
        fTotal += FP_KF * s->rotor[i] * s->rotor[i];
    }
    double velBody[3];
    rotInv(s->quat, s->vel, velBody);
    const double specFlu[3] = {
        (FP_DRAGX * velBody[0]) / FP_MASS,
        (FP_DRAGY * velBody[1]) / FP_MASS,
        (FP_DRAGZ * velBody[2] + fTotal) / FP_MASS,
    };
    accelFrd[0] = specFlu[0];
    accelFrd[1] = -specFlu[1];
    accelFrd[2] = -specFlu[2];

    *baroPa = (int32_t)lrint(FP_BARO_P0 * exp(-s->pos[2] / FP_BARO_SCALE));
}

void fpGetPoseNed(const fpSim_t *s, float posNed[3], float quatNedWxyz[4])
{
    posNed[0] = (float)s->pos[0];
    posNed[1] = (float)-s->pos[1];
    posNed[2] = (float)-s->pos[2];

    // FLU->FRD / Zup->NED is a 180-deg rotation about x; conjugate the quaternion
    // by qf = 180 about x = (x=1,y=0,z=0,w=0) in xyzw.
    const double qf[4] = {1, 0, 0, 0}; // xyzw
    const double fluXyzw[4] = {s->quat[0], s->quat[1], s->quat[2], s->quat[3]};
    double tmp[4], nedXyzw[4];
    qmul(qf, fluXyzw, tmp);   // qf (x) q
    qmul(tmp, qf, nedXyzw);   // (qf (x) q) (x) qf
    // emit wxyz
    quatNedWxyz[0] = (float)nedXyzw[3];
    quatNedWxyz[1] = (float)nedXyzw[0];
    quatNedWxyz[2] = (float)nedXyzw[1];
    quatNedWxyz[3] = (float)nedXyzw[2];
}

void fpEulerDeg(const fpSim_t *s, double *roll, double *pitch, double *yaw)
{
    float pned[3], qn[4];
    fpGetPoseNed(s, pned, qn);
    const double w = (double)qn[0], x = (double)qn[1], y = (double)qn[2], z = (double)qn[3];
    const double r2d = 180.0 / M_PI;
    *roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)) * r2d;
    double sp = 2 * (w * y - z * x);
    sp = sp > 1 ? 1 : (sp < -1 ? -1 : sp);
    *pitch = asin(sp) * r2d;
    *yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z)) * r2d;
}
