// geodesic.h — Null geodesic integration in Schwarzschild spacetime.
#pragma once

#include "config.h"
#include "math.h"

namespace bh {

/// Outcome of a single ray trace.
struct TraceResult {
    /// Classification of the ray's fate.
    enum class Kind { HitHorizon, Escaped, HitDisk } kind = Kind::Escaped;
    /// Asymptotic direction at escape (world space, unit vector).
    Vec3 finalDir{};
    /// Radial coordinate where disk was hit (if HitDisk).
    double diskRadius = 0.0;
    /// Gravitational redshift factor at hit (observed/emitted frequency).
    double redshift = 1.0;
    /// Number of RK4 steps taken.
    int steps = 0;
};

/// Trace a null geodesic from a camera position.
/// @param r0       Initial radial coordinate (M units).
/// @param theta0   Initial polar angle (radians).
/// @param phi0     Initial azimuthal angle (radians).
/// @param dir      Unit direction in the camera's local orthonormal frame
///                 (n_r, n_theta, n_phi). n_r points outward.
/// @return TraceResult with kind, final direction, disk hit info, and step count.
/// Uses MASS from config.h as the black hole mass.
TraceResult traceRay(double r0, double theta0, double phi0, const Vec3& dir) noexcept;

} // namespace bh