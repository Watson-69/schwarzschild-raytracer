// raytrace_common.h — Common ray-tracing utilities shared by CPU/GPU.
#pragma once

#include "config.h"
#include "math.h"
#include "geodesic.h"
#include "render_common.h"
#include "camera.h"
#include "multisample.h"

namespace bh {

/// Complete ray-tracing context for a single frame.
struct RaytraceContext {
    Camera camera;
    int width, height;
    int samplesPerPixel = 1;
    int maxSteps = GPU_MAX_STEPS;

    /// Trace a single ray through the scene.
    /// @param px, py   Pixel coordinates.
    /// @param jitterU, jitterV  Sub-pixel jitter [-0.5, 0.5].
    /// @return Shaded color.
    Vec3 tracePixel(int px, int py, double jitterU = 0.0, double jitterV = 0.0) const noexcept {
        const double aspect = double(width) / height;
        const double pixelSize = 2.0 / width;
        double u = (2.0 * px / (width - 1) - 1.0) * aspect + jitterU * pixelSize;
        double v = 1.0 - 2.0 * py / (height - 1) + jitterV * pixelSize;

        CameraBasis basis = camera.basis();
        Vec3 dirWorld = rayDirection(basis, u, v, CAMERA_FOV, aspect);
        Vec3 dirLocal = worldToLocalDir(dirWorld, basis);

        TraceResult res = traceRay(camera.r, camera.theta, camera.phi, dirLocal);
        return shade(res);
    }
};

/// Render a frame to a pixel buffer.
/// @param ctx        Ray-trace context.
/// @param outPixels  Output buffer (width * height Vec3).
/// @param spp        Samples per pixel.
inline void renderFrame(const RaytraceContext& ctx, std::vector<Vec3>& outPixels, int spp = 1) noexcept {
    outPixels.resize(ctx.width * ctx.height);
    for (int py = 0; py < ctx.height; ++py) {
        for (int px = 0; px < ctx.width; ++px) {
            Vec3 accum{0,0,0};
            for (int s = 0; s < spp; ++s) {
                double ju = (spp > 1) ? (std::rand() / double(RAND_MAX) - 0.5) : 0.0;
                double jv = (spp > 1) ? (std::rand() / double(RAND_MAX) - 0.5) : 0.0;
                accum += ctx.tracePixel(px, py, ju, jv);
            }
            outPixels[py * ctx.width + px] = accum * (1.0 / spp);
        }
    }
}

} // namespace bh