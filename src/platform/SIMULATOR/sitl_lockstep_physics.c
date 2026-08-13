/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <string.h>

#include "sitl_lockstep.h"
#include "sitl_lockstep_physics.h"

#define GRAVITY_MSS     9.80665
#define SEA_LEVEL_PA    101325.0
#define BARO_SCALE_M    8434.0      // isothermal atmosphere scale height

// Airframe: generic 5" quad, ~250 mm diagonal
#define MASS_KG         0.55
#define ARM_M           0.078       // motor x/y offset from CoG
#define THRUST_MAX_N    3.75        // per motor; hover at u ~= 0.60
#define TORQUE_PER_N    0.014       // prop drag torque / thrust ratio, m
#define ROTOR_TAU_S     0.025       // rotor spin-up time constant
#define DRAG_N_PER_MS   0.35        // linear aero drag
#define ANG_DRAG_1      2e-4        // angular damping, N.m.s
#define ANG_DRAG_2      1e-4        // quadratic angular damping
#define IXX             2.5e-3
#define IYY             2.5e-3
#define IZZ             4.5e-3

// Betaflight quad X, motor index order REAR_R, FRONT_R, REAR_L, FRONT_L
// (mixerQuadX in flight/mixer_init.c). Geometry in NED body axes
// (x forward, y right). Yaw reaction signs follow the default rotation
// (RR/FL clockwise from above, FR/RL counter-clockwise), which is the
// direction the firmware's mixer yaw column assumes for negative feedback.
static const double motorX[4]   = { -ARM_M, ARM_M, -ARM_M, ARM_M };
static const double motorY[4]   = { ARM_M, ARM_M, -ARM_M, -ARM_M };
static const double yawSign[4]  = { -1.0, 1.0, 1.0, -1.0 };

static void quatRotate(const double q[4], const double v[3], double out[3])
{
    // out = R(q) * v, q is body->world
    const double w = q[0], x = q[1], y = q[2], z = q[3];
    out[0] = (1 - 2 * (y * y + z * z)) * v[0] + 2 * (x * y - w * z) * v[1] + 2 * (x * z + w * y) * v[2];
    out[1] = 2 * (x * y + w * z) * v[0] + (1 - 2 * (x * x + z * z)) * v[1] + 2 * (y * z - w * x) * v[2];
    out[2] = 2 * (x * z - w * y) * v[0] + 2 * (y * z + w * x) * v[1] + (1 - 2 * (x * x + y * y)) * v[2];
}

static void quatRotateInv(const double q[4], const double v[3], double out[3])
{
    const double qc[4] = { q[0], -q[1], -q[2], -q[3] };
    quatRotate(qc, v, out);
}

// Inject the current body rates, specific force and barometric pressure
// into the firmware's virtual sensors.
static void injectSensors(const quadSim_t *sim)
{
    bflSetGyroAccel(sim->omega[0], sim->omega[1], sim->omega[2],
                    sim->accBody[0], sim->accBody[1], sim->accBody[2]);
    bflSetBaro((int32_t)lrint(SEA_LEVEL_PA * exp(sim->pos[2] / BARO_SCALE_M)));
}

void quadSimInit(quadSim_t *sim)
{
    memset(sim, 0, sizeof(*sim));
    sim->q[0] = 1.0;
    sim->onGround = true;
    sim->accBody[2] = -GRAVITY_MSS;
    // must run after firmware init (see bflYawMotorsReversed)
    sim->yawDir = bflYawMotorsReversed() ? -1.0 : 1.0;
}

void quadSimStep(quadSim_t *sim, double dt)
{
    // rotor lag on the firmware's normalised outputs
    float cmd[4];
    bflGetMotorsNormalised(cmd, 4);

    double thrustTotal = 0.0;
    for (int i = 0; i < 4; i++) {
        double u = (double)cmd[i];
        if (u < 0.0) u = 0.0;
        if (u > 1.0) u = 1.0;
        sim->rotor[i] += (u - sim->rotor[i]) * dt / (ROTOR_TAU_S + dt);
        sim->thrust[i] = THRUST_MAX_N * sim->rotor[i] * sim->rotor[i];
        thrustTotal += sim->thrust[i];
    }

    if (sim->onGround && thrustTotal <= MASS_KG * GRAVITY_MSS) {
        // sitting on the ground: no motion (omega is zero here),
        // gravity-only specific force
        const double gWorld[3] = { 0.0, 0.0, -GRAVITY_MSS };
        quatRotateInv(sim->q, gWorld, sim->accBody);
        injectSensors(sim);
        return;
    }
    sim->onGround = false;

    // body torques from motor thrusts (thrust acts along -z body)
    double tau[3] = { 0.0, 0.0, 0.0 };
    for (int i = 0; i < 4; i++) {
        tau[0] += -motorY[i] * sim->thrust[i];
        tau[1] += motorX[i] * sim->thrust[i];
        tau[2] += sim->yawDir * yawSign[i] * TORQUE_PER_N * sim->thrust[i];
    }

    // angular damping and gyroscopic term
    const double *w = sim->omega;
    const double wmag = sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    tau[0] += -(ANG_DRAG_1 + ANG_DRAG_2 * wmag) * w[0] - (IZZ - IYY) * w[1] * w[2];
    tau[1] += -(ANG_DRAG_1 + ANG_DRAG_2 * wmag) * w[1] - (IXX - IZZ) * w[2] * w[0];
    tau[2] += -(ANG_DRAG_1 + ANG_DRAG_2 * wmag) * w[2] - (IYY - IXX) * w[0] * w[1];

    sim->omega[0] += tau[0] / IXX * dt;
    sim->omega[1] += tau[1] / IYY * dt;
    sim->omega[2] += tau[2] / IZZ * dt;

    // attitude: q_dot = 0.5 * q (x) (0, omega). `w` aliases sim->omega, so
    // this deliberately integrates the just-updated rates (semi-implicit
    // Euler) — the damping terms above used the pre-update values.
    {
        const double *q = sim->q;
        const double dq[4] = {
            0.5 * (-q[1] * w[0] - q[2] * w[1] - q[3] * w[2]),
            0.5 * (q[0] * w[0] + q[2] * w[2] - q[3] * w[1]),
            0.5 * (q[0] * w[1] - q[1] * w[2] + q[3] * w[0]),
            0.5 * (q[0] * w[2] + q[1] * w[1] - q[2] * w[0]),
        };
        for (int i = 0; i < 4; i++) {
            sim->q[i] += dq[i] * dt;
        }
        const double n = sqrt(sim->q[0] * sim->q[0] + sim->q[1] * sim->q[1]
                              + sim->q[2] * sim->q[2] + sim->q[3] * sim->q[3]);
        for (int i = 0; i < 4; i++) {
            sim->q[i] /= n;
        }
    }

    // linear dynamics in world frame
    const double thrustBody[3] = { 0.0, 0.0, -thrustTotal };
    double thrustWorld[3];
    quatRotate(sim->q, thrustBody, thrustWorld);

    double accWorld[3];
    double dragWorld[3];
    for (int i = 0; i < 3; i++) {
        dragWorld[i] = -DRAG_N_PER_MS * sim->vel[i];
        accWorld[i] = (thrustWorld[i] + dragWorld[i]) / MASS_KG;
    }
    accWorld[2] += GRAVITY_MSS;

    for (int i = 0; i < 3; i++) {
        sim->vel[i] += accWorld[i] * dt;
        sim->pos[i] += sim->vel[i] * dt;
    }

    // ground contact (z >= 0 is at/below ground in NED)
    if (sim->pos[2] >= 0.0) {
        sim->pos[2] = 0.0;
        sim->vel[0] = sim->vel[1] = sim->vel[2] = 0.0;
        sim->omega[0] = sim->omega[1] = sim->omega[2] = 0.0;
        sim->onGround = true;
    }

    // accelerometer = specific force in body frame (excludes gravity)
    double dragBody[3];
    quatRotateInv(sim->q, dragWorld, dragBody);
    if (sim->onGround) {
        const double gWorld[3] = { 0.0, 0.0, -GRAVITY_MSS };
        quatRotateInv(sim->q, gWorld, sim->accBody);
    } else {
        sim->accBody[0] = dragBody[0] / MASS_KG;
        sim->accBody[1] = dragBody[1] / MASS_KG;
        sim->accBody[2] = -thrustTotal / MASS_KG + dragBody[2] / MASS_KG;
    }

    injectSensors(sim);
}

double quadSimAltitude(const quadSim_t *sim)
{
    return -sim->pos[2];
}

void quadSimEulerDeg(const quadSim_t *sim, double *roll, double *pitch, double *yaw)
{
    const double w = sim->q[0], x = sim->q[1], y = sim->q[2], z = sim->q[3];
    const double r2d = 180.0 / M_PI;
    *roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)) * r2d;
    double s = 2 * (w * y - z * x);
    if (s > 1.0) s = 1.0;
    if (s < -1.0) s = -1.0;
    *pitch = asin(s) * r2d;
    *yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z)) * r2d;
}
