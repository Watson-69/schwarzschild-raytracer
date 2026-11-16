// math.h — Small math utilities for the geodesic tracer (self-contained).
#pragma once

#include <cmath>
#include <vector>

namespace bh {

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    constexpr Vec3() noexcept = default;
    constexpr Vec3(double X, double Y, double Z) noexcept : x(X), y(Y), z(Z) {}
};

constexpr Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
constexpr Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
constexpr Vec3 operator-(const Vec3& a) noexcept { return Vec3(-a.x, -a.y, -a.z); }
constexpr Vec3 operator*(const Vec3& a, double s) noexcept {
    return Vec3(a.x * s, a.y * s, a.z * s);
}
constexpr Vec3 operator*(double s, const Vec3& a) noexcept { return a * s; }
constexpr Vec3 operator/(const Vec3& a, double s) noexcept {
    return Vec3(a.x / s, a.y / s, a.z / s);
}
inline Vec3& operator+=(Vec3& a, const Vec3& b) noexcept {
    a.x += b.x; a.y += b.y; a.z += b.z; return a;
}
inline Vec3& operator*=(Vec3& a, double s) noexcept {
    a.x *= s; a.y *= s; a.z *= s; return a;
}

constexpr double dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline double length(const Vec3& a) noexcept { return std::sqrt(dot(a, a)); }
inline Vec3 normalize(const Vec3& a) noexcept {
    const double l = length(a);
    if (l < 1e-12) return a;
    return a * (1.0 / l);
}

// Schwarzschild metric helpers (spherical coordinates: t, r, theta, phi)
// ds^2 = -(1 - 2M/r) dt^2 + (1 - 2M/r)^-1 dr^2 + r^2 dtheta^2 + r^2 sin^2 theta dphi^2
constexpr double f_metric(double r, double M) noexcept { return 1.0 - 2.0 * M / r; }

constexpr Vec3 sphToCart(double r, double theta, double phi) noexcept {
    return Vec3(r * std::sin(theta) * std::cos(phi),
                r * std::cos(theta),
                r * std::sin(theta) * std::sin(phi));
}

constexpr double PI = 3.14159265358979323846;

// A ray/geodesic state in spherical coordinates.
struct GeodesicState {
    double r = 0.0, theta = 0.0, phi = 0.0;
    double dr = 0.0, dtheta = 0.0, dphi = 0.0;
    double E = 0.0, L = 0.0;   // conserved
    double lambda = 0.0;        // affine parameter
};

} // namespace bh