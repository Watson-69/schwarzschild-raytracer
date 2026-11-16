# build.ps1 — Thin wrapper around CMake (CPU + GL targets)
$ErrorActionPreference = 'Stop'
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    cmake --preset release
    cmake --build --preset release
} else {
    Write-Host "CMake not found; building CPU-only targets with g++"
    g++ -std=c++17 -O2 -I include tests/test_geodesic.cpp src/geodesic.cpp -o bh_test.exe
    g++ -std=c++17 -O2 -I include src/render_cpu.cpp src/geodesic.cpp -o bh_cpu.exe
}