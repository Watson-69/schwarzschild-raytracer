// raytrace_integrator.h — Standalone integrator interface for reuse/testing.
#pragma once

#include "config.h"
#include "math.h"

namespace bh {

/// Function signature for acceleration computation.
/// @param s      Current geodesic state.
/// @param arr    Output: r'' acceleration.
/// @param at     Output: theta'' acceleration.
/// @param aph    Output: phi'' acceleration.
using AccelFn = void(*)(const GeodesicState&, double&, double&, double&) noexcept;

/// Perform one RK4 step with custom acceleration function.
/// @param s      Geodesic state (updated in place).
/// @param h      Step size.
/// @param accel  Acceleration function.
inline void rk4Step(GeodesicState& s, double h, AccelFn accel) noexcept {
    auto deriv = [&](const GeodesicState& st, double* d) noexcept {
        double arr, at, aph;
        accel(st, arr, at, aph);
        d[0] = st.dr; d[1] = st.dtheta; d[2] = st.dphi;
        d[3] = arr;   d[4] = at;        d[5] = aph;
    };

    const double y[6] = {s.r, s.theta, s.phi, s.dr, s.dtheta, s.dphi};
    double k1[6], k2[6], k3[6], k4[6], yt[6];
    deriv(s, k1);
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + 0.5 * h * k1[i];
    { GeodesicState t = s; t.r=yt[0]; t.theta=yt[1]; t.phi=yt[2]; t.dr=yt[3]; t.dtheta=yt[4]; t.dphi=yt[5]; deriv(t, k2); }
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + 0.5 * h * k2[i];
    { GeodesicState t = s; t.r=yt[0]; t.theta=yt[1]; t.phi=yt[2]; t.dr=yt[3]; t.dtheta=yt[4]; t.dphi=yt[5]; deriv(t, k3); }
    for (int i = 0; i < 6; ++i) yt[i] = y[i] + h * k3[i];
    { GeodesicState t = s; t.r=yt[0]; t.theta=yt[1]; t.phi=yt[2]; t.dr=yt[3]; t.dtheta=yt[4]; t.dphi=yt[5]; deriv(t, k4); }
    s.r      = y[0] + h/6.0*(k1[0]+2*k2[0]+2*k3[0]+k4[0]);
    s.theta  = y[1] + h/6.0*(k1[1]+2*k2[1]+2*k3[1]+k4[1]);
    s.phi    = y[2] + h/6.0*(k1[2]+2*k2[2]+2*k3[2]+k4[2]);
    s.dr     = y[3] + h/6.0*(k1[3]+2*k2[3]+2*k3[3]+k4[3]);
    s.dtheta = y[4] + h/6.0*(k1[4]+2*k2[4]+2*k3[4]+k4[4]);
    s.dphi   = y[5] + h/6.0*(k1[5]+2*k2[5]+2*k3[5]+k4[5]);
    s.lambda += h;
}

} // namespace bh