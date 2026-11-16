// test_metrics.cpp — Unit tests for Schwarzschild metrics.
#include "metrics.h"
#include "config.h"
#include <iostream>
#include <cmath>

using namespace bh;

bool approxEqual(double a, double b, double eps = 1e-12) {
    return std::fabs(a - b) <= eps;
}

int main() {
    int fails = 0;
    auto check = [&](const char* name, bool cond, double a = 0, double b = 0) {
        if (!cond) { std::cout << "FAIL: " << name << " (" << a << " vs " << b << ")\n"; ++fails; }
        else { std::cout << "PASS: " << name << "\n"; }
    };

    // Horizon
    check("horizon radius", approxEqual(bh::metrics::horizonRadius(MASS), SCHWARZSCHILD_R));
    // Photon sphere
    check("photon sphere", approxEqual(bh::metrics::photonSphereRadius(MASS), 3.0));
    // ISCO
    check("ISCO", approxEqual(bh::metrics::iscoRadius(MASS), 6.0));
    // Metric factor
    check("f(4M)", approxEqual(bh::metrics::f(4.0 * MASS, MASS), 0.5));
    check("f(inf)", approxEqual(bh::metrics::f(1e6, MASS), 1.0, 1e-6));
    // Redshift
    check("redshift at 4M", approxEqual(bh::metrics::redshiftFactor(4.0 * MASS, MASS), std::sqrt(0.5)));
    // Keplerian Omega
    check("Omega at 4M", approxEqual(bh::metrics::keplerianOmega(4.0 * MASS, MASS), std::sqrt(MASS / (64.0 * MASS * MASS * MASS))));
    // Proper distance
    double d = bh::metrics::properRadialDistance(4.0 * MASS, 8.0 * MASS, MASS);
    check("proper dist > coord", d > 4.0 * MASS);
    // Effective potential
    check("V_eff null at 3M", approxEqual(bh::metrics::effectivePotentialNull(3.0 * MASS, 1.0, MASS), 1.0 / (6.0 * MASS * MASS), 1e-12));

    if (fails == 0) {
        std::cout << "All metrics tests passed.\n";
        return 0;
    }
    return 1;
}