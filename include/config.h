// config.h — All tunable constants in one place.
// Adjust these to change the physics and rendering behaviour.
// Units: geometric units, G = c = 1. Lengths in units of M (mass).
#pragma once

namespace bh {

// ---- Physics -------------------------------------------------------------
/// Black hole mass M in geometric units.
constexpr double MASS = 1.0;
/// Schwarzschild radius r_s = 2M.
constexpr double SCHWARZSCHILD_R = 2.0 * MASS;

// Accretion disk (equatorial plane), radii in M.
/// Inner disk edge ~3 r_s (near ISCO at 3 r_s for Schwarzschild).
constexpr double DISK_INNER = 3.0 * SCHWARZSCHILD_R;
/// Outer disk edge ~10 r_s.
constexpr double DISK_OUTER = 10.0 * SCHWARZSCHILD_R;
/// Disk half-thickness (unused in current model).
constexpr double DISK_THICKNESS = 0.05;

// Integration
/// RK4 step size in affine parameter lambda (M units).
constexpr double STEP_SIZE = 0.05;
/// Maximum RK4 steps per ray.
constexpr int    MAX_STEPS = 20000;
/// Ray considered escaped if r exceeds this value.
constexpr double ESCAPE_RADIUS = 100.0;
/// Numerical margin around horizon for capture test.
constexpr double HORIZON_EPS = 1e-6;

// Camera
/// Default camera distance from black hole (M).
constexpr double CAMERA_DISTANCE = 30.0;
/// Field of view (degrees).
constexpr double CAMERA_FOV = 60.0;
/// Minimum polar angle (radians) to avoid poles.
constexpr double CAMERA_THETA_MIN = 0.1;
/// Maximum polar angle (radians) to avoid poles.
constexpr double CAMERA_THETA_MAX = 3.04;
/// Minimum zoom distance (M).
constexpr double CAMERA_R_MIN = 6.0;
/// Maximum zoom distance (M).
constexpr double CAMERA_R_MAX = 80.0;
/// Mouse drag sensitivity (radians per pixel).
constexpr double MOUSE_SENSITIVITY = 0.005;
/// Scroll zoom speed (M per scroll unit).
constexpr double ZOOM_SPEED = 1.5;

// Rendering
/// Window width in pixels.
constexpr int WINDOW_WIDTH = 1280;
/// Window height in pixels.
constexpr int WINDOW_HEIGHT = 720;
/// Maximum RK4 steps in GPU shader (float precision).
constexpr int GPU_MAX_STEPS = 400;

// Starfield procedural generation
/// Star grid scale factor for layer 0.
constexpr double STAR_SCALE_0 = 200.0;
/// Star grid scale factor for layer 1.
constexpr double STAR_SCALE_1 = 320.0;
/// Star grid scale factor for layer 2.
constexpr double STAR_SCALE_2 = 440.0;
/// Hash threshold for star appearance (0-1).
constexpr double STAR_THRESHOLD = 0.993;

/// Temperature falloff exponent for disk (T ~ r^-0.75).
constexpr double DISK_TEMP_EXPONENT = 0.75;

} // namespace bh