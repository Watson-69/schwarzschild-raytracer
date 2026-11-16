# build.ps1 — Thin wrapper around CMake (CPU + GL targets)
param(
    [switch]$Clean,
    [ValidateSet('debug','release')][string]$Config = 'release',
    [switch]$TestOnly,
    [switch]$CpuOnly
)
$ErrorActionPreference = 'Stop'

if ($Clean) {
    Write-Host "Cleaning build artifacts..."
    Remove-Item -Recurse -Force build, *.exe, *.ppm, *.png -ErrorAction SilentlyContinue
    exit 0
}

if (Get-Command cmake -ErrorAction SilentlyContinue) {
    if (-not $CpuOnly) {
        Write-Host "Building with CMake ($Config)..."
        cmake --preset $Config
        cmake --build --preset $Config
        if ($TestOnly) {
            ctest --preset $Config --output-on-failure
        }
    } else {
        Write-Host "CMake available but --CpuOnly specified; building CPU targets"
        g++ -std=c++17 -O2 -I include tests/test_geodesic.cpp src/geodesic.cpp -o bh_test.exe
        g++ -std=c++17 -O2 -I include src/render_cpu.cpp src/geodesic.cpp -o bh_cpu.exe
        Write-Host "Built bh_test.exe and bh_cpu.exe"
    }
} else {
    Write-Warning "CMake not found; building CPU-only targets with g++"
    g++ -std=c++17 -O2 -I include tests/test_geodesic.cpp src/geodesic.cpp -o bh_test.exe
    g++ -std=c++17 -O2 -I include src/render_cpu.cpp src/geodesic.cpp -o bh_cpu.exe
    Write-Host "Built bh_test.exe and bh_cpu.exe"
}