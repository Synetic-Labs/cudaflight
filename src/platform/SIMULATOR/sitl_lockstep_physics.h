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

// Deterministic quadrotor rigid-body model for the SITL_LOCKSTEP harness.
//
// Closes the loop: reads the firmware's normalised motor outputs, steps
// a quad X rigid body and injects the resulting gyro/acc/baro readings
// back into the firmware. All state and arithmetic are double precision;
// identical call sequences produce bit-identical trajectories.
//
// Frames are NED throughout (body: x forward, y right, z down; world:
// z down, ground at z = 0, altitude = -z), matching the injection
// conventions of bflSetGyroAccel().

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct quadSim_s {
    // rigid body state
    double q[4];        // body->world quaternion (w, x, y, z)
    double omega[3];    // body angular rate, rad/s
    double pos[3];      // world position, m (NED: pos[2] <= 0 above ground)
    double vel[3];      // world velocity, m/s
    double rotor[4];    // lagged normalised rotor command [0..1]
    double yawDir;      // +1 default prop rotation, -1 yaw_motors_reversed
    bool onGround;

    // last computed outputs (for telemetry)
    double thrust[4];   // per-motor thrust, N
    double accBody[3];  // specific force injected, m/s^2
} quadSim_t;

void quadSimInit(quadSim_t *sim);

// Advance the model by dt seconds: read motors from the firmware,
// integrate, and inject gyro/acc/baro back into the firmware.
void quadSimStep(quadSim_t *sim, double dt);

// Altitude above ground in metres (= -pos[2]).
double quadSimAltitude(const quadSim_t *sim);

// Euler angles of the body, degrees (roll right+, pitch nose-up+, yaw right+).
void quadSimEulerDeg(const quadSim_t *sim, double *roll, double *pitch, double *yaw);
