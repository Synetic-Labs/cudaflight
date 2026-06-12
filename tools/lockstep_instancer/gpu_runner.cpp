// Host runner for the GPU SITL_LOCKSTEP build.
//
// Loads the device module (real Betaflight firmware compiled for NVPTX
// plus the device flight harness), creates N firmware instances in
// device memory, flies them and applies the same oracle as the CPU
// harness: every unperturbed instance must produce a bit-identical
// motor-output hash, a perturbed one must diverge without affecting the
// others. Exit codes match sitl_lockstep_main.c (2 arm fail, 3 not
// armed/airborne, 4 hash mismatch / no divergence).

#include <cuda.h>

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
    const char *modulePath = "obj/gpu/fw.cubin";
    unsigned instances = 16;
    unsigned perturb = UINT32_MAX;
    int flySeconds = 10;
    unsigned chunkMs = 250;
    bool testReset = false;

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
        } else if (!strcmp(argv[i], "--test-reset")) {
            testReset = true;
        } else {
            fprintf(stderr,
                    "usage: %s [--instances N] [--perturb K] [--seconds N] [--chunk MS] [--module FILE] [--test-reset]\n",
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
    CU(cuCtxCreate(&ctx, 0, dev));
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

    CUfunction fInit, fBoot, fRun, fFinish, fSnapshot, fReset;
    CU(cuModuleGetFunction(&fInit, mod, "bfInstanceInit"));
    CU(cuModuleGetFunction(&fBoot, mod, "bfBoot"));
    CU(cuModuleGetFunction(&fRun, mod, "bfRun"));
    CU(cuModuleGetFunction(&fFinish, mod, "bfFinish"));
    CU(cuModuleGetFunction(&fSnapshot, mod, "bfSnapshot"));
    CU(cuModuleGetFunction(&fReset, mod, "bfReset"));

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
