// render_cpu.cpp — CPU ray tracer using shared rendering logic and image I/O.
#include "config.h"
#include "geodesic.h"
#include "render_common.h"
#include "image_io.h"
#include <vector>
#include <iostream>

using namespace bh;

int main() {
    const int W = 320, H = 180;
    const double camR = CAMERA_DISTANCE, camTheta = 1.2, camPhi = 0.0;

    CameraBasis basis = makeCameraBasis(camR, camTheta, camPhi);
    const double fov = CAMERA_FOV;
    const double aspect = double(W) / H;

    std::vector<Vec3> pixels;
    pixels.reserve(W * H);

    for (int py = 0; py < H; ++py) {
        for (int px = 0; px < W; ++px) {
            double u = (2.0 * px / (W - 1) - 1.0) * aspect;
            double v = 1.0 - 2.0 * py / (H - 1);
            Vec3 dirWorld = rayDirection(basis, u, v, CAMERA_FOV, aspect);
            Vec3 dir = worldToLocalDir(dirWorld, basis);

            TraceResult res = traceRay(camR, camTheta, camPhi, dir);
            pixels.push_back(shade(res));
        }
    }

    if (writePPM("output.ppm", pixels, W, H)) {
        std::cout << "Wrote output.ppm\n";
    }
    if (writePNG("output.png", pixels, W, H)) {
        std::cout << "Wrote output.png\n";
    }
    return 0;
}