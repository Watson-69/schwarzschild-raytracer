// disk_model.h — Accretion disk physical models.
#pragma once

#include "config.h"
#include "math.h"
#include "metrics.h"

namespace bh::disk {

/// Novikov-Thorne thin disk temperature profile.
/// T(r) = [ (3 G M Mdot) / (8 pi sigma r^3) * (1 - sqrt(r_isco/r)) ]^(1/4)
struct NovikovThorneParams {
    double M;           // Black hole mass (geometric units)
    double Mdot;        // Accretion rate (geometric units)
    double sigma = 5.670374419e-8; // Stefan-Boltzmann (SI, need conversion)
};

/// Temperature at radius r (in geometric units).
inline double novikovThorneTemp(double r, const NovikovThorneParams& p) noexcept {
    const double r_isco = bh::metrics::iscoRadius(p.M);
    const double factor = 1.0 - std::sqrt(r_isco / r);
    if (factor <= 0.0) return 0.0;
    // Simplified: T^4 ~ (M * Mdot / r^3) * (1 - sqrt(r_isco/r))
    // Omit constants for geometric units
    return std::pow(p.Mdot * factor / (r * r * r), 0.25);
}

/// Shakura-Sunyaev alpha-disk surface density.
struct AlphaDiskParams {
    double alpha = 0.1;
    double Mdot;
    double M;
};

/// Surface density profile (simplified).
inline double alphaDiskSigma(double r, const AlphaDiskParams& p) noexcept {
    const double Omega = bh::metrics::keplerianOmega(r, p.M);
    const double cs = 0.1 * std::sqrt(p.M / r); // sound speed
    const double nu = p.alpha * cs * cs / Omega;
    return p.Mdot / (3.0 * M_PI * nu);
}

/// Magnetic field strength (equipartition).
inline double magneticField(double r, double sigma, double T) noexcept {
    // B^2 / (8 pi) = a * sigma * T^4 (simplified)
    return std::sqrt(8.0 * M_PI * 7.5657e-16 * sigma * T * T * T * T);
}

/// Emission spectrum: multicolor blackbody.
/// Returns RGB color from temperature (simple mapping).
inline bh::Vec3 blackbodyColor(double T) noexcept {
    // Normalize T to [0,1] for visualization
    double t = T / 1e5; // arbitrary scale
    t = std::min(1.0, std::max(0.0, t));
    double r = t < 0.5 ? 2.0 * t : 1.0;
    double g = t < 0.5 ? 0.0 : 2.0 * (t - 0.5);
    double b = 1.0 - t;
    return bh::Vec3{r, g, b};
}

/// Beaming factor for moving emitter (Doppler + aberration).
inline double beamingFactor(double Omega, double L, double E, double r, double M) noexcept {
    const double f = bh::metrics::f(r, M);
    const double ut = 1.0 / std::sqrt(f - r * r * M / (r * r * r));
    return ut * (E - Omega * L);
}

} // namespace bh::disk