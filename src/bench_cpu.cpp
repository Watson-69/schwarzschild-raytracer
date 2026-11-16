// bench_cpu.cpp — Simple benchmark for CPU renderer.
#include "config.h"
#include "geodesic.h"
#include <chrono>
#include <iostream>

using namespace bh;

int main() {
    const int W = 320, H = 180;
    const int runs = 3;

    Vec3 e_r{std::sin(1.2), std::cos(1.2), 0.0};
    Vec3 e_th{std::cos(1.2), -std::sin(1.2), 0.0};
    Vec3 e_ph{0.0, 0.0, 1.0};
    Vec3 fwd = e_r * -1.0;
    Vec3 right = e_ph;
    Vec3 up = e_th;
    double tanHalf = std::tan(CAMERA_FOV * PI / 360.0);
    double aspect = double(W) / H;

    auto traceFrame = [&](int frame) {
        for (int py = 0; py < H; ++py) {
            for (int px = 0; px < W; ++px) {
                double u = (2.0 * px / (W - 1) - 1.0) * aspect;
                double v = 1.0 - 2.0 * py / (H - 1);
                Vec3 dirWorld = normalize(fwd + right * u * tanHalf + up * v * tanHalf);
                double nr = dot(dirWorld, e_r);
                double nth = dot(dirWorld, e_th);
                double nph = dot(dirWorld, e_ph);
                Vec3 dir{nr, nth, nph};
                traceRay(CAMERA_DISTANCE, 1.2, 0.0, dir);
            }
        }
    };

    // Warm-up
    traceFrame(-1);

    for (int r = 0; r < runs; ++r) {
        auto t0 = std::chrono::high_resolution_clock::now();
        traceFrame(r);
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double mrays = (W * H) / 1e6;
        std::cout << "Run " << r + 1 << ": " << ms << " ms  ("
                  << mrays / (ms / 1000.0) << " Mray/s)\n";
    }
    return 0;
}