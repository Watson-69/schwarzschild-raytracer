// camera.h — Camera state and manipulation.
#pragma once

#include "config.h"
#include "math.h"

namespace bh {

/// Camera with orbital controls.
struct Camera {
    double r = CAMERA_DISTANCE;
    double theta = 1.2;
    double phi = 0.0;

    /// Update from mouse drag (delta in pixels).
    void orbit(double dx, double dy) noexcept {
        phi   -= dx * MOUSE_SENSITIVITY;
        theta -= dy * MOUSE_SENSITIVITY;
        if (theta < CAMERA_THETA_MIN) theta = CAMERA_THETA_MIN;
        if (theta > CAMERA_THETA_MAX) theta = CAMERA_THETA_MAX;
    }
    /// Zoom by scroll delta (positive = zoom in).
    void zoom(double dy) noexcept {
        r -= dy * ZOOM_SPEED;
        if (r < CAMERA_R_MIN) r = CAMERA_R_MIN;
        if (r > CAMERA_R_MAX) r = CAMERA_R_MAX;
    }
    /// Reset to defaults.
    void reset() noexcept {
        r = CAMERA_DISTANCE; theta = 1.2; phi = 0.0;
    }
    /// Get world-space basis vectors.
    CameraBasis basis() const noexcept { return makeCameraBasis(r, theta, phi); }
};

} // namespace bh