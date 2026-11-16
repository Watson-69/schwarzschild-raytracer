// math.h — Small math utilities for the geodesic tracer (self-contained).
#pragma once

#include <cmath>
#include <vector>

namespace bh {

/// 3D vector with double precision.
struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    constexpr Vec3() noexcept = default;
    constexpr Vec3(double X, double Y, double Z) noexcept : x(X), y(Y), z(Z) {}
};

/// Vector addition.
constexpr Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
/// Vector subtraction.
constexpr Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
/// Unary negation.
constexpr Vec3 operator-(const Vec3& a) noexcept { return Vec3(-a.x, -a.y, -a.z); }
/// Scalar multiplication (vector * scalar).
constexpr Vec3 operator*(const Vec3& a, double s) noexcept {
    return Vec3(a.x * s, a.y * s, a.z * s);
}
/// Scalar multiplication (scalar * vector).
constexpr Vec3 operator*(double s, const Vec3& a) noexcept { return a * s; }
/// Scalar division.
constexpr Vec3 operator/(const Vec3& a, double s) noexcept {
    return Vec3(a.x / s, a.y / s, a.z / s);
}
/// In-place addition.
inline Vec3& operator+=(Vec3& a, const Vec3& b) noexcept {
    a.x += b.x; a.y += b.y; a.z += b.z; return a;
}
/// In-place scalar multiplication.
inline Vec3& operator*=(Vec3& a, double s) noexcept {
    a.x *= s; a.y *= s; a.z *= s; return a;
}

/// Dot product.
constexpr double dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
/// Euclidean length.
inline double length(const Vec3& a) noexcept { return std::sqrt(dot(a, a)); }
/// Normalize to unit length; returns zero vector if length is near zero.
inline Vec3 normalize(const Vec3& a) noexcept {
    const double l = length(a);
    if (l < 1e-12) return a;
    return a * (1.0 / l);
}

/// Schwarzschild metric factor f(r) = 1 - 2M/r.
constexpr double f_metric(double r, double M) noexcept { return 1.0 - 2.0 * M / r; }

/// Convert spherical (r, theta, phi) to Cartesian.
/// Theta is polar angle from +y axis; phi is azimuthal around y.
constexpr Vec3 sphToCart(double r, double theta, double phi) noexcept {
    return Vec3(r * std::sin(theta) * std::cos(phi),
                r * std::cos(theta),
                r * std::sin(theta) * std::sin(phi));
}

constexpr double PI = 3.14159265358979323846;

/// Full state of a null geodesic in spherical coordinates.
struct GeodesicState {
    double r = 0.0, theta = 0.0, phi = 0.0;      // position
    double dr = 0.0, dtheta = 0.0, dphi = 0.0;   // velocity d/dlambda
    double E = 0.0, L = 0.0;                     // conserved energy, ang. momentum
    double lambda = 0.0;                          // affine parameter
};

} // namespace bh