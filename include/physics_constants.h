// physics_constants.h — Physical constants with SI units for reference.
#pragma once

namespace bh::physics {

/// Speed of light in m/s.
constexpr double C = 299792458.0;
/// Gravitational constant in m^3 kg^-1 s^-2.
constexpr double G = 6.67430e-11;
/// Solar mass in kg.
constexpr double M_SUN = 1.98847e30;
/// Schwarzschild radius of one solar mass in meters.
constexpr double RS_SUN = 2.0 * G * M_SUN / (C * C);

/// Convert geometric mass M (in meters) to kg.
inline double massGeometricToKg(double M_meters) {
    return M_meters * C * C / G;
}

/// Convert geometric length (meters) to Schwarzschild radii.
inline double metersToRs(double meters, double M_meters) {
    return meters / (2.0 * M_meters);
}

/// Convert Schwarzschild radii to meters.
inline double rsToMeters(double rs, double M_meters) {
    return rs * 2.0 * M_meters;
}

} // namespace bh::physics