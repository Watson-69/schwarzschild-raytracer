// render_common.h — Shared rendering helpers (camera basis, color mapping).
#pragma once

#include "config.h"
#include "geodesic.h"

namespace bh {

/// Camera basis vectors in world space.
struct CameraBasis {
    Vec3 e_r, e_th, e_ph;   // orthonormal: e_r outward, e_theta south, e_phi east
    Vec3 fwd, right, up;    // view basis: fwd = -e_r, right = e_phi, up = e_th
};

/// Build camera basis from spherical position.
inline CameraBasis makeCameraBasis(double r, double theta, double phi) noexcept {
    CameraBasis b;
    b.e_r = Vec3{std::sin(theta)*std::cos(phi), std::cos(theta), std::sin(theta)*std::sin(phi)};
    b.e_th = Vec3{std::cos(theta)*std::cos(phi), -std::sin(theta), std::cos(theta)*std::sin(phi)};
    b.e_ph = Vec3{-std::sin(phi), 0.0, std::cos(phi)};
    b.fwd = b.e_r * -1.0;
    b.right = b.e_ph;
    b.up = b.e_th;
    return b;
}

/// Compute world-space ray direction for a pixel.
/// @param b      Camera basis.
/// @param u,v    NDC coordinates in [-1,1] (u right, v up).
/// @param fov    Vertical field of view in degrees.
/// @param aspect Width/height ratio.
inline Vec3 rayDirection(const CameraBasis& b, double u, double v, double fovDeg, double aspect) noexcept {
    double tanHalf = std::tan(fovDeg * PI / 360.0);
    return normalize(b.fwd + b.right * u * tanHalf * aspect + b.up * v * tanHalf);
}

/// Convert world direction to local spherical components at camera.
/// @param d      World direction (unit).
/// @param b      Camera basis.
/// @return (n_r, n_theta, n_phi) in local orthonormal frame.
inline Vec3 worldToLocalDir(const Vec3& d, const CameraBasis& b) noexcept {
    return Vec3{dot(d, b.e_r), dot(d, b.e_th), dot(d, b.e_ph)};
}

/// Procedural starfield color from asymptotic direction.
inline Vec3 starfield(const Vec3& d) noexcept {
    double th = std::acos(std::max(-1.0, std::min(1.0, d.y)));
    double ph = std::atan2(d.z, d.x);
    double u = ph / (2 * PI) + 0.5, v = th / PI;
    Vec3 color{0,0,0};
    for (int i = 0; i < 3; ++i) {
        double scale = (i == 0) ? STAR_SCALE_0 : (i == 1 ? STAR_SCALE_1 : STAR_SCALE_2);
        double gx = std::floor(u * scale), gy = std::floor(v * scale);
        double h = std::sin(gx * 12.9898 + gy * 78.233 + i * 37.719) * 43758.5453;
        h = h - std::floor(h);
        if (h > STAR_THRESHOLD) {
            double b = (h - STAR_THRESHOLD) * 142.857;
            color.x += b * 0.7; color.y += b * 0.85; color.z += b;
        }
    }
    double band = std::exp(-std::pow((th - PI * 0.5) * 3.5, 2.0)) * 0.12;
    color.x += 0.35 * band; color.y += 0.45 * band; color.z += 0.75 * band;
    return color;
}

/// Accretion disk color from radius and redshift factor.
inline Vec3 diskColor(double r, double g) noexcept {
    double t = std::pow(DISK_INNER / r, DISK_TEMP_EXPONENT);
    double cool[3] = {0.85, 0.25, 0.05};
    double hot[3]  = {1.0, 0.92, 0.72};
    Vec3 base{
        cool[0] + (hot[0] - cool[0]) * std::max(0.0, std::min(1.0, t)),
        cool[1] + (hot[1] - cool[1]) * std::max(0.0, std::min(1.0, t)),
        cool[2] + (hot[2] - cool[2]) * std::max(0.0, std::min(1.0, t))
    };
    double intensity = std::pow(std::max(g, 0.0), 3.0);
    if (intensity > 6.0) intensity = 6.0;
    base.x *= (0.25 + 0.75 * intensity);
    base.y *= (0.25 + 0.75 * intensity);
    base.z *= (0.25 + 0.75 * intensity);
    return base;
}

/// Shade a TraceResult into an RGB color.
inline Vec3 shade(const TraceResult& res) noexcept {
    if (res.kind == TraceResult::Kind::HitHorizon) return Vec3{0,0,0};
    if (res.kind == TraceResult::Kind::Escaped) return starfield(res.finalDir);
    return diskColor(res.diskRadius, res.redshift);
}

} // namespace bh