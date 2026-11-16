# Schwarzschild Black Hole Renderer

A C++17 / OpenGL 3.3 real-time renderer of a Schwarzschild black hole. Light is
traced as null geodesics in the Schwarzschild metric using RK4 integration.
Includes an accretion disk with gravitational redshift and Doppler beaming, a
procedural starfield, mouse orbit / scroll zoom, and an FPS counter in the title.

## Features

- Null-geodesic ray tracing in the Schwarzschild metric (RK4, `G = c = 1`)
- Event-horizon capture, escape to a procedural starfield, and thin-disk hits
- Accretion disk: temperature gradient, gravitational redshift, Doppler beaming
- Interactive camera: left-drag to orbit, scroll to zoom
- FPS displayed in the window title
- CPU-only fallback that writes a PPM/PNG frame (no OpenGL needed)
- Unit tests for the geodesic integrator

## Build (CMake — interactive OpenGL version)

Requirements: CMake >= 3.16, a C++17 compiler, OpenGL 3.3. GLFW and GLM are
fetched automatically.

```sh
cmake --preset release
cmake --build --preset release
```

Or manually:

```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Run (interactive)

```sh
./build/release/blackhole      # Linux/macOS
.\build\release\blackhole.exe # Windows
```

Mouse: drag to orbit, scroll to zoom.

## Build & run without CMake (CPU only)

```powershell
.\build.ps1
.\bh_test.exe
.\bh_cpu.exe   # writes output.ppm / output.png
```

## Tests

```sh
ctest --preset release
# or: ./bh_test
```

Verifies photons far from the hole travel nearly straight and that photons aimed
at the photon sphere are captured (small impact parameter) or escape (large).

## Sample output

CPU renderer (no OpenGL) produces a static frame:

![output.png](output.png)

The CMake/OpenGL build produces an interactive window instead.

## Configuration

All tunable constants (mass, disk radii, camera distance, step size, ...) live in
`include/config.h`.

## Simplifications

- Geodesic integration uses `float` in the GPU shader for performance; the CPU
  reference and tests use `double`.
- Shader caps integration at ~400 steps per ray for interactive frame rates.
- Disk is treated as infinitesimally thin and opaque.
- Accretion disk angular velocity is Keplerian.
- Starfield is procedural (hash-based), not a real sky catalog.
- Camera is a static observer (no orbital motion).
- Fixed RK4 step size (configurable).