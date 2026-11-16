// test_geodesic.cpp
#include "geodesic.h"
#include "config.h"
#include <iostream>
#include <cmath>

using namespace bh;

int main() {
    int failures = 0;
    auto check = [&](const char* name, bool cond) {
        if (!cond) { std::cout << "FAIL: " << name << "\n"; ++failures; }
        else { std::cout << "PASS: " << name << "\n"; }
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
        // deflection should be small: final direction z-component near initial nphi
        check("deflection small", std::fabs(res.finalDir.z - dir.z) < 0.05);
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    }
    std::cout << failures << " test(s) failed.\n";
    return 1;
}