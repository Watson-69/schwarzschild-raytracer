// geodesic.cpp — Null geodesic integration in Schwarzschild spacetime (CPU, double).
//
// Physics: Schwarzschild metric (G=c=1): ds^2 = -f dt^2 + f^-1 dr^2
//        + r^2(dtheta^2 + sin^2 theta dphi^2), f = 1 - 2M/r.
// Null geodesics: x^mu'' + Gamma^mu_{nu rho} x^nu' x^rho' = 0.
// Conserved: E = f dt/dlambda, L = r^2 sin^2 theta dphi/dlambda.
// Integration in spherical spatial coords (r,theta,phi) with RK4.
#include "geodesic.h"
#include "config.h"
#include <cmath>

namespace bh {

namespace {
constexpr double M = MASS;

void accel(const GeodesicState& s, double& arr, double& at, double& ap) noexcept {
    const double r = s.r;
    const double th = s.theta;
    const double f = 1.0 - 2.0 * M / r;
    double sinth = std::sin(th);
    if (std::fabs(sinth) < 1e-12) sinth = 1e-12; // guard poles
    arr = M * s.dr * s.dr / (r * r * f)
        + r * f * (s.dtheta * s.dtheta)
        + r * f * sinth * sinth * (s.dphi * s.dphi)
        - M * s.E * s.E / (r * r * f);
    at = -2.0 * s.dr * s.dtheta / r + std::sin(th) * std::cos(th) * (s.dphi * s.dphi);
    ap = -2.0 * s.dr * s.dphi / r - 2.0 * (std::cos(th) / sinth) * s.dtheta * s.dphi;
}

void rk4Step(GeodesicState& s, double h) noexcept {
    auto deriv = [](const GeodesicState& st, double* d) noexcept {
        double arr, at, ap;
        accel(st, arr, at, ap);
        d[0] = st.dr; d[1] = st.dtheta; d[2] = st.dphi;
        d[3] = arr;   d[4] = at;        d[5] = ap;
    };

    const double y[6] = {s.r, s.theta, s.phi, s.dr, s.dtheta, s.dphi};
    double k1[6], k2[6], k3[6], k4[6], yt[6];
    deriv(s, k1);
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + 0.5 * h * k1[i];
    {
        GeodesicState t = s;
        t.r = yt[0]; t.theta = yt[1]; t.phi = yt[2];
        t.dr = yt[3]; t.dtheta = yt[4]; t.dphi = yt[5];
        deriv(t, k2);
    }
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + 0.5 * h * k2[i];
    {
        GeodesicState t = s;
        t.r = yt[0]; t.theta = yt[1]; t.phi = yt[2];
        t.dr = yt[3]; t.dtheta = yt[4]; t.dphi = yt[5];
        deriv(t, k3);
    }
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + h * k3[i];
    {
        GeodesicState t = s;
        t.r = yt[0]; t.theta = yt[1]; t.phi = yt[2];
        t.dr = yt[3]; t.dtheta = yt[4]; t.dphi = yt[5];
        deriv(t, k4);
    }
    s.r      = y[0] + h / 6.0 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
    s.theta  = y[1] + h / 6.0 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
    s.phi    = y[2] + h / 6.0 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
    s.dr     = y[3] + h / 6.0 * (k1[3] + 2 * k2[3] + 2 * k3[3] + k4[3]);
    s.dtheta = y[4] + h / 6.0 * (k1[4] + 2 * k2[4] + 2 * k3[4] + k4[4]);
    s.dphi   = y[5] + h / 6.0 * (k1[5] + 2 * k2[5] + 2 * k3[5] + k4[5]);
    s.lambda += h;
}
} // namespace

TraceResult traceRay(double r0, double theta0, double phi0, const Vec3& dir) noexcept {
    TraceResult res{};
    res.steps = 0;

    const double f0 = 1.0 - 2.0 * M / r0;
    const double sinth0 = std::sin(theta0);
    const double nr = dir.x, nth = dir.y, nph = dir.z;

    GeodesicState s{};
    s.r = r0; s.theta = theta0; s.phi = phi0;
    s.dr = std::sqrt(f0) * nr;
    s.dtheta = nth / r0;
    s.dphi = nph / (r0 * sinth0);
    s.E = std::sqrt(f0);
    s.L = r0 * sinth0 * nph;
    s.lambda = 0.0;

    bool prevAboveDisk = (s.theta > PI / 2.0);
    const double h = STEP_SIZE;

    for (int i = 0; i < MAX_STEPS; ++i) {
        const double r = s.r;
        if (r <= SCHWARZSCHILD_R + HORIZON_EPS) {
            res.kind = TraceResult::Kind::HitHorizon;
            res.steps = i;
            return res;
        }
        if (r >= ESCAPE_RADIUS) {
            res.kind = TraceResult::Kind::Escaped;
            res.steps = i;
            const double f = 1.0 - 2.0 * M / r;
            double nr_f = s.dr / std::sqrt(f);
            const double nth_f = r * s.dtheta;
            const double nph_f = r * std::sin(s.theta) * s.dphi;
            double len = std::sqrt(nr_f * nr_f + nth_f * nth_f + nph_f * nph_f);
            if (len < 1e-12) len = 1.0;
            nr_f /= len;
            const double nth_n = nth_f / len;
            const double nph_n = nph_f / len;
            const double th = s.theta, ph = s.phi;
            const Vec3 e_r(std::sin(th) * std::cos(ph), std::cos(th), std::sin(th) * std::sin(ph));
            const Vec3 e_t(std::cos(th) * std::cos(ph), -std::sin(th), std::cos(th) * std::sin(ph));
            const Vec3 e_p(-std::sin(ph), 0.0, std::cos(ph));
            res.finalDir = nr_f * e_r + nth_n * e_t + nph_n * e_p;
            return res;
        }

        const bool nowAbove = (s.theta > PI / 2.0);
        if (nowAbove != prevAboveDisk) {
            const double rc = s.r;
            if (rc >= DISK_INNER && rc <= DISK_OUTER) {
                res.kind = TraceResult::Kind::HitDisk;
                res.steps = i;
                res.diskRadius = rc;
                const double fcam = 1.0 - 2.0 * M / r0;
                const double fdisk = 1.0 - 2.0 * M / rc;
                const double Omega = std::sqrt(M / (rc * rc * rc));
                double denom = fdisk - rc * rc * Omega * Omega;
                if (denom < 1e-12) denom = 1e-12;
                const double ut = 1.0 / std::sqrt(denom);
                const double femit = (s.E - Omega * s.L) * ut;
                const double femitSafe = (std::fabs(femit) < 1e-12)
                    ? (femit < 0.0 ? -1e-12 : 1e-12) : femit;
                const double fobs = s.E / std::sqrt(fcam);
                res.redshift = fobs / femitSafe;
                return res;
            }
        }
        prevAboveDisk = nowAbove;

        rk4Step(s, h);
        res.steps = i;
    }
    res.kind = TraceResult::Kind::Escaped;
    res.steps = MAX_STEPS;
    return res;
}

} // namespace bh
