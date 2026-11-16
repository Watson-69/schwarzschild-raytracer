// benchmark.cpp — Comprehensive benchmark suite.
#include "config.h"
#include "geodesic.h"
#include "render_common.h"
#include "raytrace_common.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace bh;

void benchGeodesic() {
    const int N = 10000;
    Vec3 e_r{std::sin(1.2), std::cos(1.2), 0.0};
    Vec3 e_th{std::cos(1.2), -std::sin(1.2), 0.0};
    Vec3 e_ph{0.0, 0.0, 1.0};
    Vec3 fwd = e_r * -1.0;
    Vec3 right = e_ph;
    Vec3 up = e_th;
    double tanHalf = std::tan(CAMERA_FOV * PI / 360.0);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        double u = (i % 320) * 2.0 / 319 - 1.0;
        double v = (i / 320) * 2.0 / 179 - 1.0;
        Vec3 dirWorld = normalize(fwd + right * u * tanHalf + up * v * tanHalf);
        Vec3 dir{dot(dirWorld, e_r), dot(dirWorld, e_th), dot(dirWorld, e_ph)};
        traceRay(CAMERA_DISTANCE, 1.2, 0.0, dir);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Geodesic: " << N << " rays in " << ms << " ms ("
              << N / (ms / 1000.0) << " rays/s)\n";
}

void benchRender() {
    const int W = 320, H = 180;
    RaytraceContext ctx;
    ctx.camera.r = CAMERA_DISTANCE;
    ctx.camera.theta = 1.2;
    ctx.camera.phi = 0.0;
    ctx.width = W; ctx.height = H;
    ctx.samplesPerPixel = 1;
    ctx.maxSteps = MAX_STEPS;

    std::vector<Vec3> pixels;
    auto start = std::chrono::high_resolution_clock::now();
    renderFrame(ctx, pixels, 1);
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double mrays = (W * H) / 1e6;
    std::cout << "Render: " << W << "x" << H << " in " << ms << " ms ("
              << mrays / (ms / 1000.0) << " Mray/s)\n";
}

int main() {
    std::cout << "=== Benchmark Suite ===\n";
    benchGeodesic();
    benchRender();
    return 0;
}