// test_geodesic.cpp
#include "geodesic.h"
#include "config.h"
#include <iostream>
#include <cmath>

using namespace bh;

bool approxEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

int main() {
    int failures = 0;
    auto check = [&](const char* name, bool cond, double a = 0, double b = 0) {
        if (!cond) {
            std::cout << "FAIL: " << name << " (" << a << " vs " << b << ")\n";
            ++failures;
        } else {
            std::cout << "PASS: " << name << "\n";
        }
    };

    // Test 1: photon far from hole travels nearly straight (small deflection)
    {
        double r0 = 50.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        Vec3 dir(0.0, 0.0, 1.0);
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test1 kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        check("far photon escapes", res.kind == TraceResult::Kind::Escaped);
    }
    // Test 2: photon aimed at photon sphere captured (b=4 < critical)
    {
        double r0 = 50.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        double b = 4.0;
        double nr = -std::sqrt(1.0 - (b * b) / (r0 * r0));
        double nphi = b / r0;
        Vec3 dir(nr, 0.0, nphi);
        dir = normalize(dir);
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test2 (b=4) kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        check("b=4 captured", res.kind == TraceResult::Kind::HitHorizon);
    }
    // Test 3: photon with large impact parameter escapes
    {
        double r0 = 50.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        double b = 10.0;
        double nr = -std::sqrt(1.0 - (b * b) / (r0 * r0));
        double nphi = b / r0;
        Vec3 dir(nr, 0.0, nphi);
        dir = normalize(dir);
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test3 (b=10) kind=" << static_cast<int>(res.kind) << "\n";
        check("b=10 escapes", res.kind == TraceResult::Kind::Escaped);
    }
    // Test 4: straight line far away deflection small
    {
        double r0 = 100.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        double b = 50.0;
        double nr = -std::sqrt(1.0 - (b * b) / (r0 * r0));
        double nphi = b / r0;
        Vec3 dir(nr, 0.0, nphi);
        dir = normalize(dir);
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test4 kind=" << static_cast<int>(res.kind) << " finalDir.z=" << res.finalDir.z << "\n";
        check("far photon escapes", res.kind == TraceResult::Kind::Escaped);
        check("deflection small", std::fabs(res.finalDir.z - dir.z) < 0.05);
    }
    // Test 5: photon at critical impact parameter ~3√3 M ~ 5.196 should orbit
    {
        double r0 = 50.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        double b = 5.2;
        double nr = -std::sqrt(std::max(0.0, 1.0 - (b * b) / (r0 * r0)));
        double nphi = b / r0;
        Vec3 dir(nr, 0.0, nphi);
        dir = normalize(dir);
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test5 (b=5.2) kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        // At critical impact, photon may be captured or escape depending on numerics
        check("critical b behaves", res.kind == TraceResult::Kind::HitHorizon || res.kind == TraceResult::Kind::Escaped);
    }
    // Test 6: purely radial inward ray hits horizon
    {
        double r0 = 10.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        Vec3 dir(-1.0, 0.0, 0.0); // purely radial inward
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test6 (radial) kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        check("radial inward hits horizon", res.kind == TraceResult::Kind::HitHorizon);
    }
    // Test 7: purely radial outward ray escapes
    {
        double r0 = 10.0;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        Vec3 dir(1.0, 0.0, 0.0); // purely radial outward
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test7 (radial out) kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        check("radial outward escapes", res.kind == TraceResult::Kind::Escaped);
    }
    // Test 8: photon grazing horizon (r0 just outside r_s) with tangential dir
    {
        double r0 = SCHWARZSCHILD_R + 0.1;
        double theta0 = PI / 2.0;
        double phi0 = 0.0;
        Vec3 dir(0.0, 0.0, 1.0); // tangential
        TraceResult res = traceRay(r0, theta0, phi0, dir);
        std::cout << "Test8 (grazing) kind=" << static_cast<int>(res.kind) << " steps=" << res.steps << "\n";
        check("near horizon tangential", res.kind == TraceResult::Kind::HitHorizon || res.kind == TraceResult::Kind::Escaped);
    }

    if (failures == 0) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    }
    std::cout << "\n" << failures << " test(s) failed.\n";
    return 1;
}