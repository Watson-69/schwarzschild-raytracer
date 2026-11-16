// test_disk.cpp — Unit tests for disk models.
#include "disk_model.h"
#include "config.h"
#include <iostream>
#include <cmath>

using namespace bh;

bool approxEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

int main() {
    int fails = 0;
    auto check = [&](const char* name, bool cond, double a = 0, double b = 0) {
        if (!cond) { std::cout << "FAIL: " << name << " (" << a << " vs " << b << ")\n"; ++fails; }
        else { std::cout << "PASS: " << name << "\n"; }
    };

    bh::disk::NovikovThorneParams p{1.0, 1e-3};
    double T = bh::disk::novikovThorneTemp(6.0, p);
    check("NT temp at ISCO=0", approxEqual(T, 0.0));
    T = bh::disk::novikovThorneTemp(10.0, p);
    check("NT temp at 10M > 0", T > 0.0);

    bh::disk::AlphaDiskParams ap{0.1, 1e-3, 1.0};
    double sig = bh::disk::alphaDiskSigma(10.0, ap);
    check("Sigma > 0", sig > 0.0);

    if (fails == 0) {
        std::cout << "All disk tests passed.\n";
        return 0;
    }
    return 1;
}