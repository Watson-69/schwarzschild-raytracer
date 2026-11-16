// render_cpu.cpp — CPU ray tracer: writes output.ppm (no OpenGL/GLFW needed).
#include "config.h"
#include "geodesic.h"
#include "render_common.h"
#include <cmath>
#include <fstream>
#include <iostream>

using namespace bh;

int main() {
    int W = 320, H = 180;
    double camR = CAMERA_DISTANCE, camTheta = 1.2, camPhi = 0.0;

    CameraBasis basis = makeCameraBasis(camR, camTheta, camPhi);
    double fov = CAMERA_FOV;
    double aspect = double(W) / H;

    std::ofstream out("output.ppm");
    out << "P3\n" << W << " " << H << "\n255\n";

    for (int py = 0; py < H; ++py) {
        for (int px = 0; px < W; ++px) {
            double u = (2.0 * px / (W - 1) - 1.0) * aspect;
            double v = 1.0 - 2.0 * py / (H - 1);
            Vec3 dirWorld = rayDirection(basis, u, v, fov, aspect);
            Vec3 dir = worldToLocalDir(dirWorld, basis);

            TraceResult res = traceRay(camR, camTheta, camPhi, dir);
            Vec3 color = shade(res);

            int r = int(std::min(1.0, color.x) * 255.0);
            int g = int(std::min(1.0, color.y) * 255.0);
            int b = int(std::min(1.0, color.z) * 255.0);
            out << r << " " << g << " " << b << "\n";
        }
    }
    out.close();
    std::cout << "Wrote output.ppm\n";
    return 0;
}