// multisample.h — Temporal accumulation and multisampling for renderer.
#pragma once

#include "config.h"
#include "math.h"
#include "render_common.h"

namespace bh {

/// Accumulation buffer for temporal anti-aliasing.
struct AccumBuffer {
    std::vector<Vec3> color;
    std::vector<int>  count;
    int width = 0, height = 0;

    AccumBuffer() = default;
    AccumBuffer(int w, int h) { resize(w, h); }

    void resize(int w, int h) {
        width = w; height = h;
        color.assign(w * h, Vec3{0,0,0});
        count.assign(w * h, 0);
    }

    void addSample(int px, int py, const Vec3& sample) {
        int idx = py * width + px;
        color[idx] += sample;
        count[idx] += 1;
    }

    Vec3 getAverage(int px, int py) const {
        int idx = py * width + px;
        int n = count[idx];
        return n > 0 ? color[idx] * (1.0 / n) : Vec3{0,0,0};
    }

    void clear() {
        std::fill(color.begin(), color.end(), Vec3{0,0,0});
        std::fill(count.begin(), count.end(), 0);
    }
};

/// Jittered sample pattern for multisampling.
inline Vec3 jitteredDir(const CameraBasis& b, double u, double v, double fov, double aspect,
                        double jitterU, double jitterV, double pixelSize) noexcept {
    double ju = u + jitterU * pixelSize;
    double jv = v + jitterV * pixelSize;
    return rayDirection(b, ju, jv, fov, aspect);
}

} // namespace bh