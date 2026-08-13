// Real-time pose server for an external renderer client — see README.md.
//
// Boots ONE real Betaflight firmware instance + a first-principles rigid body on
// the CPU, settles+arms it, then serves a request/response loop over localhost
// TCP: a driver sends RC channels and a substep count, this advances the
// firmware+physics in lockstep and replies with the drone pose (and, on demand,
// the OSD grid). At a single instance this native path runs far faster than
// real time, unlike the GPU cudaflight path built to amortise kernel launches
// over thousands of drones.
//
// Built as a normal SITL executable by build.sh, which reuses the exact
// firmware link (LTO + sitl.ld) — see the rationale there.
//
// Wire protocol (little-endian; both ends are x86):
//   request  (fixed 58 B): u8 substeps, u8 flags (bit0 want_osd, bit1 reset),
//                          16 x u16 rc (us), 3 x f32 impulse_lin (m/s, NED world),
//                          3 x f32 impulse_ang (rad/s, body)
//   response (variable):   u8 armed, 3 x f32 pos_ned, 4 x f32 quat_wxyz,
//                          3 x f32 euler_deg (roll,pitch,yaw),
//                          u16 osd_len, u8 rows, u8 cols, osd_len bytes
// Single instance => no IR instancer (bflInstanceTemplateFixup is a no-op).

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "platform.h"

#include "sitl_lockstep.h"
#include "sitl_lockstep_instance.h"
#include "firstprinciples_physics.h"

// Firmware init entry points (pulling in fc/init.h drags platform headers;
// declare directly, as device_flight.c and bfcpu.c do).
void systemInit(void);
void initPhase1(void);
void initPhase2(void);
void initPhase3(void);
void printfSerialInit(void);
void bflOsdApplyDemoLayoutIfBlank(void);
void bflOsdDefaultCraftName(const char *name);
// flight/imu.c (ENABLE_SIMULATOR): overwrite the estimator's quaternion.
void imuSetAttitudeQuat(float w, float x, float y, float z);

#define CONTROL_STEP_US 1000 // inject sensors/RC and step the firmware at 1 kHz
#define SETTLE_SECONDS  6    // gyro calibration + arming boot grace period
#define ARM_SECONDS     1
#define DEFAULT_PORT    5556
#define RESET_ALT_M     10.0 // height the drone respawns at on a reset request

static fpSim_t g_fp;
static uint16_t g_rc[BFL_MAX_RC_CHANNELS];

// Standard Tait-Bryan ZYX (yaw*pitch*roll) euler (degrees) -> body->world
// quaternion (w,x,y,z). Inverse of fpEulerDeg's extraction, so a round-trip
// reproduces the input angles.
static void rpyDegToQuatWxyz(double rollDeg, double pitchDeg, double yawDeg, float q[4])
{
    const double d2r = M_PI / 180.0;
    const double cr = cos(rollDeg * d2r * 0.5), sr = sin(rollDeg * d2r * 0.5);
    const double cp = cos(pitchDeg * d2r * 0.5), sp = sin(pitchDeg * d2r * 0.5);
    const double cy = cos(yawDeg * d2r * 0.5), sy = sin(yawDeg * d2r * 0.5);
    q[0] = (float)(cr * cp * cy + sr * sp * sy);
    q[1] = (float)(sr * cp * cy - cr * sp * sy);
    q[2] = (float)(cr * sp * cy + sr * cp * sy);
    q[3] = (float)(cr * cp * sy - sr * sp * cy);
}

// One control step: physics turns the previous step's motor outputs into this
// step's sensor readings, then the firmware advances 1 ms against them.
static void controlStep(void)
{
    float motors[4];
    bflGetMotorsNormalised(motors, 4);
    fpStep(&g_fp, motors, CONTROL_STEP_US * 1e-6);

    double gyro[3], accel[3];
    int32_t baro;
    fpGetImu(&g_fp, gyro, accel, &baro);
    bflSetGyroAccel(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
    bflSetBaro(baro);

    // Pin the firmware attitude to the physics ground truth so the OSD horizon
    // always matches the true pose: the on-board estimator (USE_IMU_CALC)
    // otherwise drifts during maneuvers and gets stuck off-level after a reset.
    // Betaflight's attitude convention has pitch inverted vs the pose
    // (nose-down positive), so rebuild the quat with pitch negated or the OSD
    // horizon moves the wrong way.
    double er, ep, ey;
    fpEulerDeg(&g_fp, &er, &ep, &ey);
    float q[4];
    rpyDegToQuatWxyz(er, -ep, ey, q);
    imuSetAttitudeQuat(q[0], q[1], q[2], q[3]);

    bflSetRc(g_rc, BFL_MAX_RC_CHANNELS);
    bflStepUs(CONTROL_STEP_US);
}

// Boot, settle (gyro cal) and arm the instance. Returns true if armed.
static bool boot(const char *eepromPath)
{
    bflInstanceTemplateFixup(); // no-op in this non-instanced build
    if (eepromPath) {
        bflSetEepromPath(eepromPath);
    }

#if defined(SERIAL_PORT_COUNT) && SERIAL_PORT_COUNT > 0
    printfSerialInit();
#endif
    systemInit();
    initPhase1();
    initPhase2();
    initPhase3();

    bflConfigureArmSwitch();  // ARM on AUX1 (rc[4])
    bflConfigureModeSwitch(); // ANGLE on AUX2 (rc[5]); low => acro
    bflOsdApplyDemoLayoutIfBlank();
    bflOsdDefaultCraftName("BETAFLIGHT");

    for (int i = 0; i < BFL_MAX_RC_CHANNELS; i++) {
        g_rc[i] = 1500;
    }
    g_rc[2] = 1000; // throttle idle
    for (int i = 4; i < BFL_MAX_RC_CHANNELS; i++) {
        g_rc[i] = 1000; // aux low
    }

    fpInit(&g_fp);

    for (int ms = 0; ms < SETTLE_SECONDS * 1000; ms++) {
        controlStep();
    }
    g_rc[4] = 1800; // AUX1 high -> arm
    for (int ms = 0; ms < ARM_SECONDS * 1000; ms++) {
        controlStep();
    }
    return bflIsArmed();
}

static bool readn(int fd, void *buf, size_t n)
{
    uint8_t *p = buf;
    while (n > 0) {
        const ssize_t r = read(fd, p, n);
        if (r < 0 && errno == EINTR) {
            continue;
        }
        if (r <= 0) {
            return false;
        }
        p += r;
        n -= (size_t)r;
    }
    return true;
}

static bool writen(int fd, const void *buf, size_t n)
{
    const uint8_t *p = buf;
    while (n > 0) {
        const ssize_t w = write(fd, p, n);
        if (w < 0 && errno == EINTR) {
            continue;
        }
        if (w <= 0) {
            return false;
        }
        p += w;
        n -= (size_t)w;
    }
    return true;
}

// Serve one connected client until it disconnects.
static void serveClient(int fd)
{
    const int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    for (;;) {
        // req: substeps, flags, 16 RC channels, then an external kick from the
        // outside world (collisions / explosions / wind): a linear velocity
        // impulse (m/s, NED world) and an angular-rate impulse (rad/s, body).
        uint8_t req[2 + 2 * 16 + 24];
        if (!readn(fd, req, sizeof(req))) {
            return; // client gone
        }
        const uint8_t substeps = req[0];
        const uint8_t flags = req[1];
        const bool wantOsd = flags & 0x01;
        const bool doReset = flags & 0x02;
        // RC values are taken as-is: the client is a trusted localhost peer
        for (int i = 0; i < 16 && i < BFL_MAX_RC_CHANNELS; i++) {
            uint16_t v;
            memcpy(&v, &req[2 + 2 * i], 2);
            g_rc[i] = v;
        }
        float impLin[3], impAng[3];
        memcpy(impLin, &req[2 + 2 * 16], 12);
        memcpy(impAng, &req[2 + 2 * 16 + 12], 12);

        if (doReset) {
            // Teleport the rigid body back to a clean, level airborne hover (zero
            // velocity and body rates, at RESET_ALT_M) without rebooting the
            // firmware, so a crashed / stuck / lost drone recovers instantly. The
            // firmware keeps its armed state; its attitude estimate reconverges
            // from the now-level gyro/accel within a few steps.
            fpReset(&g_fp, RESET_ALT_M);
        }

        // Apply the external impulse: a one-shot kick to velocity (world) and body
        // rate. The sim integrates the result on real firmware dynamics, so the
        // drone reacts to (and must recover from) blasts and impacts. A sustained
        // force (e.g. wind) is just a small impulse sent every step (force * dt).
        if (impLin[0] || impLin[1] || impLin[2] || impAng[0] || impAng[1] || impAng[2]) {
            fpImpulse(&g_fp, impLin, impAng);
        }

        for (int i = 0; i < substeps; i++) {
            controlStep();
        }

        uint8_t resp[1 + 12 + 16 + 12 + 2 + 1 + 1 + 2048]; // 2048 caps the OSD payload
        size_t o = 0;
        resp[o++] = bflIsArmed() ? 1 : 0;
        float posNed[3], quatNed[4];
        fpGetPoseNed(&g_fp, posNed, quatNed);
        memcpy(&resp[o], posNed, 12);
        o += 12;
        memcpy(&resp[o], quatNed, 16); // body->world quaternion (w,x,y,z), NED/FRD
        o += 16;
        // Authoritative attitude (roll right+, pitch nose-up+, yaw right+, deg) —
        // ground truth of where the drone points, for control-vs-render debugging.
        double roll, pitch, yaw;
        fpEulerDeg(&g_fp, &roll, &pitch, &yaw);
        const float eul[3] = {(float)roll, (float)pitch, (float)yaw};
        memcpy(&resp[o], eul, sizeof(eul));
        o += sizeof(eul);

        uint16_t osdLen = 0;
        uint8_t rows = 0, cols = 0;
        if (wantOsd) {
            rows = (uint8_t)bflOsdRows();
            cols = (uint8_t)bflOsdCols();
            osdLen = (uint16_t)(rows * cols);
            if (osdLen > sizeof(resp) - (o + 4)) { // never overrun resp
                osdLen = 0;
                rows = cols = 0;
            }
        }
        memcpy(&resp[o], &osdLen, 2);
        o += 2;
        resp[o++] = rows;
        resp[o++] = cols;
        if (osdLen) {
            memcpy(&resp[o], bflOsdScreen(), osdLen);
            o += osdLen;
        }

        if (!writen(fd, resp, o)) {
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN); // a vanished client must not kill the server

    int port = DEFAULT_PORT;
    const char *eeprom = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
            if (port < 1 || port > 65535) {
                fprintf(stderr, "invalid --port '%s'\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--eeprom") == 0 && i + 1 < argc) {
            eeprom = argv[++i];
        } else {
            fprintf(stderr, "usage: %s [--port N] [--eeprom FILE]\n", argv[0]);
            return 1;
        }
    }

    setvbuf(stdout, NULL, _IOLBF, 0);

    printf("[bfserver] booting firmware (settle %ds + arm %ds)...\n",
           SETTLE_SECONDS, ARM_SECONDS);
    if (!boot(eeprom)) {
        fprintf(stderr, "[bfserver] FAILED to arm during boot\n");
        return 2;
    }
    printf("[bfserver] armed at t=%ums; osd %ux%u\n",
           (unsigned)(bflMicros() / 1000), bflOsdRows(), bflOsdCols());

    // Boot rests the drone on the ground (rotors stopped), so the floor clamp is
    // needed up to here. Now that it's armed and flies airborne, drop the floor:
    // the client world's terrain is the ground and collisions arrive as impulses,
    // so a fixed floor at the sim origin would pin the drone to its spawn height.
    fpSetFloorEnabled(false);

    const int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) {
        perror("socket");
        return 1;
    }
    const int one = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons((uint16_t)port);
    if (bind(srv, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        return 1;
    }
    if (listen(srv, 1) != 0) {
        perror("listen");
        return 1;
    }
    printf("[bfserver] listening on 127.0.0.1:%d (Ctrl-C to stop)\n", port);

    for (;;) {
        const int fd = accept(srv, NULL, NULL);
        if (fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            return 1;
        }
        printf("[bfserver] client connected\n");
        serveClient(fd);
        close(fd);
        printf("[bfserver] client disconnected; waiting for reconnect\n");
    }
}
