// interpolation.h — Smooth camera interpolation and keyframe animation.
#pragma once

#include "camera.h"
#include "math.h"

namespace bh {

/// Interpolate camera position between two keyframes.
/// @param a        Start camera.
/// @param b        End camera.
/// @param t        Interpolation parameter [0,1].
/// @return Interpolated camera.
inline Camera lerpCamera(const Camera& a, const Camera& b, double t) noexcept {
    Camera c;
    c.r = a.r + (b.r - a.r) * t;
    c.theta = a.theta + (b.theta - a.theta) * t;
    // Shortest path for phi (handle wrap-around)
    double dphi = b.phi - a.phi;
    if (dphi > M_PI) dphi -= 2 * M_PI;
    if (dphi < -M_PI) dphi += 2 * M_PI;
    c.phi = a.phi + dphi * t;
    return c;
}

/// Smoothstep interpolation.
inline double smoothstep(double t) noexcept {
    return t * t * (3.0 - 2.0 * t);
}

/// Camera animation controller.
struct CameraAnimator {
    Camera start, end;
    double duration = 0.0;
    double elapsed = 0.0;
    bool playing = false;

    void play(const Camera& s, const Camera& e, double dur) {
        start = s; end = e; duration = dur; elapsed = 0.0; playing = true;
    }

    Camera update(double dt) noexcept {
        if (!playing) return end;
        elapsed += dt;
        if (elapsed >= duration) { playing = false; return end; }
        double t = smoothstep(elapsed / duration);
        return lerpCamera(start, end, t);
    }
};

} // namespace bh