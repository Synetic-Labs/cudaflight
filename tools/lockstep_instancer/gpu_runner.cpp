// Host runner for the GPU SITL_LOCKSTEP build.
//
// Loads the device module (real Betaflight firmware compiled for NVPTX
// plus the device flight harness), creates N firmware instances in
// device memory, flies them and applies the same oracle as the CPU
// harness: every unperturbed instance must produce a bit-identical
// motor-output hash, a perturbed one must diverge without affecting the
// others. Exit codes match sitl_lockstep_main.c (2 arm fail, 3 not
// armed/airborne, 4 hash mismatch / no divergence), plus 5 (--test-reset
// fail) and 6 (--test-step fail).

#include <cuda.h>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define CU(call) \
    do { \
        CUresult rc_ = (call); \
        if (rc_ != CUDA_SUCCESS) { \
            const char *msg = nullptr; \
            cuGetErrorString(rc_, &msg); \
            fprintf(stderr, "[gpu] %s failed: %s (%d)\n", #call, msg ? msg : "?", (int)rc_); \
            exit(1); \
        } \
    } while (0)

static CUdeviceptr globalAddr(CUmodule mod, const char *name, size_t *size = nullptr)
{
    CUdeviceptr p;
    size_t sz;
    CUresult rc = cuModuleGetGlobal(&p, &sz, mod, name);
    if (rc != CUDA_SUCCESS) {
        fprintf(stderr, "[gpu] global %s not found in module\n", name);
        exit(1);
    }
    if (size) {
        *size = sz;
    }
    return p;
}

static uint64_t readU64(CUmodule mod, const char *name)
{
    uint64_t v = 0;
    CU(cuMemcpyDtoH(&v, globalAddr(mod, name), sizeof(v)));
    return v;
}

int main(int argc, char **argv)
{
    const char *modulePath = "obj/gpu/fw.fatbin";
    const char *eepromPath = nullptr;
    unsigned instances = 16;
    unsigned perturb = UINT32_MAX;
    int flySeconds = 10;
    unsigned chunkMs = 250;
    bool testReset = false;
    bool testStep = false;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--instances") && i + 1 < argc) {
            instances = (unsigned)atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--perturb") && i + 1 < argc) {
            perturb = (unsigned)atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--seconds") && i + 1 < argc) {
            flySeconds = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--chunk") && i + 1 < argc) {
            chunkMs = (unsigned)atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--module") && i + 1 < argc) {
            modulePath = argv[++i];
        } else if (!strcmp(argv[i], "--eeprom") && i + 1 < argc) {
            eepromPath = argv[++i];
        } else if (!strcmp(argv[i], "--test-reset")) {
            testReset = true;
        } else if (!strcmp(argv[i], "--test-step")) {
            testStep = true;
        } else {
            fprintf(stderr,
                    "usage: %s [--instances N] [--perturb K] [--seconds N] [--chunk MS] [--module FILE] [--eeprom FILE] [--test-reset] [--test-step]\n",
                    argv[0]);
            return 1;
        }
    }

    CU(cuInit(0));
    CUdevice dev;
    CU(cuDeviceGet(&dev, 0));
    char name[128] = {0};
    cuDeviceGetName(name, sizeof(name), dev);
    CUcontext ctx;
#if CUDA_VERSION >= 13000
    CU(cuCtxCreate(&ctx, NULL, 0, dev)); // 13.x: cuCtxCreate is the 4-arg _v4
#else
    CU(cuCtxCreate(&ctx, 0, dev));
#endif
    // The whole firmware runs on one thread's stack. The driver reserves
    // stack for the device-wide max resident thread count (~260k on a
    // 5090), so this can't be extravagant: 32 KB ≈ 8 GB reserved.
    CU(cuCtxSetLimit(CU_LIMIT_STACK_SIZE, 32 * 1024));

    CUmodule mod;
    CU(cuModuleLoad(&mod, modulePath));

    const uint64_t imageSize = readU64(mod, "__bf_image_size");
    uint64_t align = readU64(mod, "__bf_image_align");
    if (align < 256) {
        align = 256; // cuMemAlloc returns >=256-aligned; keep deltas congruent
    }
    const uint64_t stride = (imageSize + align - 1) & ~(align - 1);
    const uint64_t stateSize = readU64(mod, "__bf_state_size");
    printf("[gpu] %s, image %llu B, stride %llu B, state %llu B, %u instance(s)\n",
           name, (unsigned long long)imageSize, (unsigned long long)stride,
           (unsigned long long)stateSize, instances);

    CUdeviceptr instBuf, stateBuf, hashBuf, altBuf, armedBuf;
    CU(cuMemAlloc(&instBuf, stride * instances));
    CU(cuMemAlloc(&stateBuf, stateSize * instances));
    CU(cuMemAlloc(&hashBuf, 8 * instances));
    CU(cuMemAlloc(&altBuf, 4 * instances));
    CU(cuMemAlloc(&armedBuf, instances));

    {
        char *base = (char *)instBuf;
        CU(cuMemcpyHtoD(globalAddr(mod, "__bf_inst_base"), &base, sizeof(base)));
        CU(cuMemcpyHtoD(globalAddr(mod, "__bf_inst_stride"), &stride, sizeof(stride)));
        CU(cuMemcpyHtoD(globalAddr(mod, "__bf_inst_count"), &instances, sizeof(instances)));
    }

    CUfunction fInit, fBoot, fRun, fFinish, fSnapshot, fReset, fStep;
    CU(cuModuleGetFunction(&fInit, mod, "bfInstanceInit"));
    CU(cuModuleGetFunction(&fBoot, mod, "bfBoot"));
    CU(cuModuleGetFunction(&fRun, mod, "bfRun"));
    CU(cuModuleGetFunction(&fFinish, mod, "bfFinish"));
    CU(cuModuleGetFunction(&fSnapshot, mod, "bfSnapshot"));
    CU(cuModuleGetFunction(&fReset, mod, "bfReset"));
    CU(cuModuleGetFunction(&fStep, mod, "bfStep"));

    const unsigned block = 32;
    const unsigned grid = (instances + block - 1) / block;
    auto launch = [&](CUfunction f, void **args) {
        CU(cuLaunchKernel(f, grid, 1, 1, block, 1, 1, 0, 0, args, nullptr));
        CU(cuCtxSynchronize());
    };
    auto runMs = [&](unsigned ms) {
        for (unsigned done = 0; done < ms;) {
            unsigned step = ms - done < chunkMs ? ms - done : chunkMs;
            void *args[] = { &stateBuf, &step };
            launch(fRun, args);
            done += step;
        }
    };
    auto readHashes = [&](std::vector<uint64_t> &out) {
        void *args[] = { &stateBuf, &hashBuf, &altBuf, &armedBuf };
        launch(fFinish, args);
        out.resize(instances);
        CU(cuMemcpyDtoH(out.data(), hashBuf, 8 * instances));
    };

    {
        void *args[] = { &stateBuf, &perturb };
        launch(fInit, args);
        printf("[gpu] instances created\n");
    }
    if (eepromPath) {
        // preload a boot-ready config image (from the CPU --cli-dump
        // converter) into every instance's RAM EEPROM before boot
        FILE *f = fopen(eepromPath, "rb");
        if (!f) {
            fprintf(stderr, "[gpu] cannot open eeprom image '%s'\n", eepromPath);
            return 1;
        }
        fseek(f, 0, SEEK_END);
        const uint64_t eeLen = (uint64_t)ftell(f);
        rewind(f);
        std::vector<uint8_t> ee(eeLen);
        if (fread(ee.data(), 1, eeLen, f) != eeLen) {
            fprintf(stderr, "[gpu] failed to read '%s'\n", eepromPath);
            fclose(f);
            return 1;
        }
        fclose(f);

        CUfunction fLoadEeprom;
        CU(cuModuleGetFunction(&fLoadEeprom, mod, "bfLoadEeprom"));
        CUdeviceptr eeBuf;
        CU(cuMemAlloc(&eeBuf, eeLen));
        CU(cuMemcpyHtoD(eeBuf, ee.data(), eeLen));
        uint64_t len = eeLen;
        uint32_t perInstance = 0;
        void *args[] = { &eeBuf, &len, &perInstance };
        launch(fLoadEeprom, args);
        CU(cuMemFree(eeBuf));
        printf("[gpu] eeprom preloaded from '%s' (%llu bytes, broadcast)\n",
               eepromPath, (unsigned long long)eeLen);
    }
    {
        void *args[] = { &stateBuf };
        launch(fBoot, args);
        printf("[gpu] all instances booted\n");
    }

    if (testReset) {
        // Oracle for snapshot/restore: after a post-arm snapshot, a reset
        // instance must replay the exact same hash trajectory; a masked
        // reset must leave unflagged instances untouched.
        CUdeviceptr snapBuf, snapStBuf, flagsBuf;
        CU(cuMemAlloc(&snapBuf, stride * instances));
        CU(cuMemAlloc(&snapStBuf, stateSize * instances));
        CU(cuMemAlloc(&flagsBuf, instances));

        runMs(7000); // settle + arm, same schedule as the flight script
        {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf };
            launch(fSnapshot, args);
            printf("[reset-test] snapshot taken at t=7.0s (armed)\n");
        }

        std::vector<uint64_t> h1, h3, r1, r3, m1;
        runMs(1000);
        readHashes(h1);
        runMs(2000);
        readHashes(h3);

        std::vector<uint8_t> all(instances, 1);
        CU(cuMemcpyHtoD(flagsBuf, all.data(), instances));
        {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf, &flagsBuf };
            launch(fReset, args);
        }
        runMs(1000);
        readHashes(r1);
        runMs(2000);
        readHashes(r3);

        // masked reset: even instances replay, odd instances keep flying
        std::vector<uint8_t> even(instances);
        for (unsigned k = 0; k < instances; k++) {
            even[k] = (k % 2 == 0);
        }
        CU(cuMemcpyHtoD(flagsBuf, even.data(), instances));
        {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf, &flagsBuf };
            launch(fReset, args);
        }
        runMs(1000);
        readHashes(m1);

        bool ok = true;
        for (unsigned k = 0; k < instances; k++) {
            if (r1[k] != h1[k] || r3[k] != h3[k]) {
                fprintf(stderr, "[reset-test] instance %u: replay mismatch (%016llx vs %016llx @1s, %016llx vs %016llx @3s)\n",
                        k, (unsigned long long)r1[k], (unsigned long long)h1[k],
                        (unsigned long long)r3[k], (unsigned long long)h3[k]);
                ok = false;
            }
            const bool replayed = (m1[k] == h1[k]);
            if (even[k] && !replayed) {
                fprintf(stderr, "[reset-test] instance %u: masked reset did not replay\n", k);
                ok = false;
            }
            if (!even[k] && replayed) {
                fprintf(stderr, "[reset-test] instance %u: unflagged instance was reset\n", k);
                ok = false;
            }
        }
        printf("[reset-test] full reset replay: %s\n", ok ? "bit-exact" : "FAILED");

        // reset throughput: the per-RL-episode cost
        CU(cuMemcpyHtoD(flagsBuf, all.data(), instances));
        const int iters = 100;
        CUevent ev0, ev1;
        CU(cuEventCreate(&ev0, 0));
        CU(cuEventCreate(&ev1, 0));
        CU(cuEventRecord(ev0, 0));
        for (int i = 0; i < iters; i++) {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf, &flagsBuf };
            CU(cuLaunchKernel(fReset, grid, 1, 1, block, 1, 1, 0, 0, args, nullptr));
        }
        CU(cuEventRecord(ev1, 0));
        CU(cuCtxSynchronize());
        float ms = 0;
        CU(cuEventElapsedTime(&ms, ev0, ev1));
        printf("[reset-test] full-fleet reset: %.1f us for %u instances (%.1f ns/instance)\n",
               1000.0f * ms / iters, instances, 1e6f * ms / iters / instances);

        printf("[reset-test] %s\n", ok ? "PASS" : "FAIL");
        return ok ? 0 : 5;
    }

    if (testStep) {
        // Oracle for the gym-style step interface: (A) identical action
        // sequences replay bit-exactly across a reset; (B) a host-side
        // P-controller can hover the firmware at 5m purely through the
        // obs/action buffers; (C) throttle cut crashes, dones raise, and
        // bfReset(dones) restores the crashed instances.
        const uint64_t actDim = readU64(mod, "__bf_act_dim");
        const uint64_t obsDim = readU64(mod, "__bf_obs_dim");
        printf("[step-test] act_dim=%llu obs_dim=%llu\n",
               (unsigned long long)actDim, (unsigned long long)obsDim);

        CUdeviceptr snapBuf, snapStBuf, actBuf, obsBuf, rewBuf, doneBuf;
        CU(cuMemAlloc(&snapBuf, stride * instances));
        CU(cuMemAlloc(&snapStBuf, stateSize * instances));
        CU(cuMemAlloc(&actBuf, 4 * actDim * instances));
        CU(cuMemAlloc(&obsBuf, 4 * obsDim * instances));
        CU(cuMemAlloc(&rewBuf, 4 * instances));
        CU(cuMemAlloc(&doneBuf, instances));

        std::vector<float> act(actDim * instances, 0.0f);
        std::vector<float> obs(obsDim * instances);
        std::vector<uint8_t> done(instances);
        uint32_t decimation = 10;

        auto step = [&]() {
            CU(cuMemcpyHtoD(actBuf, act.data(), 4 * actDim * instances));
            void *args[] = { &stateBuf, &actBuf, &obsBuf, &rewBuf, &doneBuf, &decimation };
            launch(fStep, args);
            CU(cuMemcpyDtoH(obs.data(), obsBuf, 4 * obsDim * instances));
            CU(cuMemcpyDtoH(done.data(), doneBuf, instances));
        };
        auto setThrottle = [&](unsigned k, float v) { act[k * actDim + 2] = v; };
        auto altitude = [&](unsigned k) { return -obs[k * obsDim + 2]; };
        auto resetWith = [&](CUdeviceptr flags) {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf, &flags, };
            launch(fReset, args);
        };

        runMs(7000); // settle + arm
        {
            void *args[] = { &stateBuf, &snapStBuf, &snapBuf };
            launch(fSnapshot, args);
        }
        CUdeviceptr allFlags;
        CU(cuMemAlloc(&allFlags, instances));
        {
            std::vector<uint8_t> all(instances, 1);
            CU(cuMemcpyHtoD(allFlags, all.data(), instances));
        }

        bool ok = true;

        // (A) determinism across reset
        std::vector<uint64_t> hA;
        std::vector<float> obsA;
        for (int pass = 0; pass < 2; pass++) {
            if (pass == 1) {
                resetWith(allFlags);
            }
            for (int i = 0; i < 200; i++) {
                const float thr = 0.36f + 0.1f * sinf(0.1f * (float)i);
                for (unsigned k = 0; k < instances; k++) {
                    setThrottle(k, thr);
                }
                step();
            }
            std::vector<uint64_t> h;
            readHashes(h);
            if (pass == 0) {
                hA = h;
                obsA = obs;
            } else {
                for (unsigned k = 0; k < instances; k++) {
                    if (h[k] != hA[k]) {
                        fprintf(stderr, "[step-test] A: instance %u hash replay mismatch\n", k);
                        ok = false;
                    }
                }
                if (memcmp(obs.data(), obsA.data(), 4 * obsDim * instances) != 0) {
                    fprintf(stderr, "[step-test] A: obs replay mismatch\n");
                    ok = false;
                }
            }
        }
        printf("[step-test] A determinism-across-reset: %s\n", ok ? "bit-exact" : "FAILED");

        // (B) closed-loop hover at 5m through the obs/action interface
        resetWith(allFlags);
        for (unsigned k = 0; k < instances; k++) {
            setThrottle(k, 0.6f); // initial climb before first obs
        }
        for (int i = 0; i < 1000; i++) { // 10s sim at decimation 10
            step();
            for (unsigned k = 0; k < instances; k++) {
                const float alt = altitude(k);
                const float vzUp = -obs[k * obsDim + 5];
                float thr = 0.36f + 0.22f * (5.0f - alt) - 0.12f * vzUp;
                setThrottle(k, thr < -1.0f ? -1.0f : (thr > 1.0f ? 1.0f : thr));
            }
        }
        {
            float meanAlt = 0;
            bool anyDone = false, inBand = true;
            for (unsigned k = 0; k < instances; k++) {
                meanAlt += altitude(k);
                anyDone = anyDone || done[k];
                inBand = inBand && altitude(k) > 3.5f && altitude(k) < 6.5f;
            }
            meanAlt /= (float)instances;
            printf("[step-test] B hover-P-controller: mean alt %.2fm after 10s, dones=%d, in-band=%d\n",
                   (double)meanAlt, (int)anyDone, (int)inBand);
            ok = ok && !anyDone && inBand;
        }

        // (C) throttle cut -> crash -> dones as reset mask -> restored
        for (unsigned k = 0; k < instances; k++) {
            setThrottle(k, -1.0f);
        }
        int crashSteps = 0;
        bool allDone = false;
        for (; crashSteps < 300 && !allDone; crashSteps++) {
            step();
            allDone = true;
            for (unsigned k = 0; k < instances; k++) {
                allDone = allDone && done[k];
            }
        }
        printf("[step-test] C crash: all done after %d steps (%.1fs fall): %s\n",
               crashSteps, crashSteps * decimation / 1000.0, allDone ? "yes" : "NO");
        ok = ok && allDone;

        resetWith(doneBuf); // auto-reset composition: dones are the mask
        for (unsigned k = 0; k < instances; k++) {
            setThrottle(k, 0.36f);
        }
        step();
        {
            bool grounded = true, anyDone = false;
            for (unsigned k = 0; k < instances; k++) {
                grounded = grounded && altitude(k) < 0.5f;
                anyDone = anyDone || done[k];
            }
            printf("[step-test] C restore: grounded=%d dones=%d\n", (int)grounded, (int)anyDone);
            ok = ok && grounded && !anyDone;
        }

        printf("[step-test] %s\n", ok ? "PASS" : "FAIL");
        return ok ? 0 : 6;
    }

    const unsigned totalMs = (6 + 1 + (unsigned)flySeconds) * 1000; // settle + arm + fly
    for (unsigned done = 0; done < totalMs;) {
        unsigned step = totalMs - done < chunkMs ? totalMs - done : chunkMs;
        void *args[] = { &stateBuf, &step };
        launch(fRun, args);
        done += step;
        if (done % 1000 == 0 || done == totalMs) {
            printf("\r[gpu] t=%5.1fs / %.1fs", done / 1000.0, totalMs / 1000.0);
            fflush(stdout);
        }
    }
    printf("\n");

    {
        void *args[] = { &stateBuf, &hashBuf, &altBuf, &armedBuf };
        launch(fFinish, args);
    }

    std::vector<uint64_t> hash(instances);
    std::vector<float> alt(instances);
    std::vector<uint8_t> armed(instances);
    CU(cuMemcpyDtoH(hash.data(), hashBuf, 8 * instances));
    CU(cuMemcpyDtoH(alt.data(), altBuf, 4 * instances));
    CU(cuMemcpyDtoH(armed.data(), armedBuf, instances));

    // Verdict, mirroring the CPU harness
    bool allArmedAirborne = true;
    bool unperturbedIdentical = true;
    uint64_t refHash = 0;
    bool haveRef = false;
    for (unsigned k = 0; k < instances; k++) {
        const bool isPerturbed = (k == perturb);
        allArmedAirborne = allArmedAirborne && armed[k] && alt[k] > 1.0f;
        if (!isPerturbed) {
            if (!haveRef) {
                refHash = hash[k];
                haveRef = true;
            } else if (hash[k] != refHash) {
                unperturbedIdentical = false;
            }
        }
        printf("[gpu] instance %u: armed=%d alt=%7.2fm hash=%016llx%s\n",
               k, (int)armed[k], (double)alt[k], (unsigned long long)hash[k],
               isPerturbed ? " (perturbed)" : "");
    }
    printf("[gpu] done: instances=%u identical=%d\n", instances, (int)unperturbedIdentical);
    printf("GPU_TRACE_HASH: %016llx\n", (unsigned long long)refHash);

    bool anyArmed = false;
    for (unsigned k = 0; k < instances; k++) {
        anyArmed = anyArmed || armed[k];
    }
    if (!anyArmed) {
        fprintf(stderr, "[gpu] no instance armed\n");
        return 2;
    }
    if (perturb != UINT32_MAX && perturb < instances && hash[perturb] == refHash) {
        fprintf(stderr, "[gpu] PERTURBED instance did not diverge\n");
        return 4;
    }
    if (!unperturbedIdentical) {
        fprintf(stderr, "[gpu] HASH MISMATCH between unperturbed instances\n");
        return 4;
    }
    return allArmedAirborne ? 0 : 3;
}
