// adaptive_integrator.h — Adaptive step-size RK45 for geodesics.
#pragma once

#include "config.h"
#include "math.h"

namespace bh {

/// Adaptive RK4(5) step with error control.
/// @param s          Geodesic state (updated).
/// @param h          Current step size (updated).
/// @param accel      Acceleration function.
/// @param tol        Target local truncation error.
/// @param h_min      Minimum step size.
/// @param h_max      Maximum step size.
/// @return true if step accepted, false if rejected (h reduced).
bool adaptiveStep(GeodesicState& s, double& h,
                  void(*)(const GeodesicState&, double&, double&, double&) noexcept,
                  double tol = 1e-8, double h_min = 1e-4, double h_max = 0.2) noexcept;

} // namespace bh