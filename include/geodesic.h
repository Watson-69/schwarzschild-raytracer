// geodesic.h — Null geodesic integration in Schwarzschild spacetime.
#pragma once

#include "config.h"
#include "math.h"

namespace bh {

// Result of tracing a single ray.
struct TraceResult {
    enum class Kind { HitHorizon, Escaped, HitDisk } kind = Kind::Escaped;
    Vec3 finalDir{};        // direction used to sample background (if escaped)
    double diskRadius = 0.0; // r where disk was hit (if HitDisk)
    double redshift = 1.0;   // gravitational redshift factor at hit
    int steps = 0;
};

// Trace a ray from (r0, theta0, phi0) with initial unit direction `dir`
// in the local orthonormal frame of the camera. Mass is taken from config.
TraceResult traceRay(double r0, double theta0, double phi0, const Vec3& dir) noexcept;

} // namespace bh