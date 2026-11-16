// metrics.h — Schwarzschild metric calculations and derived quantities.
#pragma once

#include "config.h"
#include "math.h"

namespace bh::metrics {

/// Event horizon radius.
constexpr double horizonRadius(double M) noexcept { return 2.0 * M; }

/// Photon sphere radius (unstable circular orbits).
constexpr double photonSphereRadius(double M) noexcept { return 3.0 * M; }

/// Innermost stable circular orbit (ISCO) for massive particles.
constexpr double iscoRadius(double M) noexcept { return 6.0 * M; }

/// Metric factor f(r) = 1 - 2M/r.
constexpr double f(double r, double M) noexcept { return 1.0 - 2.0 * M / r; }

/// Inverse metric factor.
constexpr double fInv(double r, double M) noexcept { return 1.0 / f(r, M); }

/// Proper radial distance between two radii (integral of sqrt(fInv) dr).
/// Approximate analytic form for Schwarzschild.
inline double properRadialDistance(double r1, double r2, double M) noexcept {
    if (r1 > r2) std::swap(r1, r2);
    const double r_s = 2.0 * M;
    const double u1 = std::sqrt(1.0 - r_s / r1);
    const double u2 = std::sqrt(1.0 - r_s / r2);
    return r2 * u2 - r1 * u1 + r_s * std::log((1.0 + u2) / (1.0 + u1) * (1.0 - u1) / (1.0 - u2));
}

/// Gravitational redshift factor for static observer at r.
constexpr double redshiftFactor(double r, double M) noexcept {
    return std::sqrt(f(r, M));
}

/// Orbital velocity for circular Keplerian orbit at r (in local frame).
constexpr double keplerianVelocity(double r, double M) noexcept {
    return std::sqrt(M / r) / std::sqrt(1.0 - 2.0 * M / r);
}

/// Angular velocity of Keplerian orbit.
constexpr double keplerianOmega(double r, double M) noexcept {
    return std::sqrt(M / (r * r * r));
}

/// Four-velocity of a Keplerian orbit (u^t, u^phi).
struct FourVelocity {
    double ut, uphi;
};

/// Four-velocity for circular equatorial Keplerian orbit.
inline FourVelocity keplerianFourVelocity(double r, double M) noexcept {
    const double f_r = f(r, M);
    const double Omega = keplerianOmega(r, M);
    const double denom = std::sqrt(f_r - r * r * Omega * Omega);
    return { 1.0 / denom, Omega / denom };
}

/// Effective potential for null geodesics: V(r) = L^2 / (2 r^2) * f(r).
inline double effectivePotentialNull(double r, double L, double M) noexcept {
    return L * L * f(r, M) / (2.0 * r * r);
}

/// Deflection angle for large impact parameter (weak field).
constexpr double deflectionAngleWeak(double b, double M) noexcept {
    return 4.0 * M / b;
}

/// Impact parameter for given asymptotic angle and radius.
inline double impactParameter(double r, double theta, double phi, double nr, double nth, double nph, double M) noexcept {
    return r * std::sin(theta) * std::sqrt(nth * nth + nph * nph);
}

} // namespace bh::metrics