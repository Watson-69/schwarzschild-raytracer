// config.h — All tunable constants in one place.
// Adjust these to change the physics and rendering behaviour.
// Units: geometric units, G = c = 1. Lengths in units of M (mass).
#pragma once

namespace bh {

// ---- Physics -------------------------------------------------------------
constexpr double MASS = 1.0;                    // black hole mass M
constexpr double SCHWARZSCHILD_R = 2.0 * MASS;  // r_s = 2M

// Accretion disk (equatorial plane), radii in M.
constexpr double DISK_INNER = 3.0 * SCHWARZSCHILD_R;   // ~3 r_s
constexpr double DISK_OUTER = 10.0 * SCHWARZSCHILD_R;  // ~10 r_s
constexpr double DISK_THICKNESS = 0.05;                // half-thickness (M)

// Integration
constexpr double STEP_SIZE = 0.05;          // geodesic step (M)
constexpr int    MAX_STEPS = 20000;         // max steps per ray
constexpr double ESCAPE_RADIUS = 100.0;     // ray escaped beyond this r (M)
constexpr double HORIZON_EPS = 1e-6;        // margin around r_s

// Camera
constexpr double CAMERA_DISTANCE = 30.0;   // default distance (M)
constexpr double CAMERA_FOV = 60.0;         // degrees
constexpr double CAMERA_THETA_MIN = 0.1;   // clamp (rad)
constexpr double CAMERA_THETA_MAX = 3.04;  // clamp (rad)
constexpr double CAMERA_R_MIN = 6.0;       // zoom limit (M)
constexpr double CAMERA_R_MAX = 80.0;      // zoom limit (M)
constexpr double MOUSE_SENSITIVITY = 0.005;
constexpr double ZOOM_SPEED = 1.5;

// Rendering
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int GPU_MAX_STEPS = 400;          // shader step cap

// Starfield
constexpr double STAR_SCALE_0 = 200.0;
constexpr double STAR_SCALE_1 = 320.0;
constexpr double STAR_SCALE_2 = 440.0;
constexpr double STAR_THRESHOLD = 0.993;

// Disk temperature
constexpr double DISK_TEMP_EXPONENT = 0.75;

} // namespace bh
