// image_io.h — Image I/O utilities (PPM, PNG via stb).
#pragma once

#include "math.h"
#include <string>
#include <vector>

namespace bh {

/// Save RGB image as binary PPM (P6).
/// @param path       Output file path.
/// @param data       Flattened RGB array (width*height*3 bytes).
/// @param width      Image width.
/// @param height     Image height.
/// @return true on success.
bool savePPM(const std::string& path, const std::vector<unsigned char>& data, int width, int height) noexcept;

/// Convert float [0,1] RGB to byte [0,255] with sRGB gamma.
inline unsigned char toSRGB(float c) noexcept {
    c = c <= 0.0f ? 0.0f : (c >= 1.0f ? 1.0f : c);
    return static_cast<unsigned char>(std::round((c <= 0.0031308f ? 12.92f * c : 1.055f * std::pow(c, 1.0f/2.4f) - 0.055f) * 255.0f));
}

/// Convert linear RGB Vec3 to sRGB bytes.
inline std::array<unsigned char, 3> toSRGB(const Vec3& c) noexcept {
    return { toSRGB((float)c.x), toSRGB((float)c.y), toSRGB((float)c.z) };
}

/// Write float RGB image as PPM with sRGB encoding.
bool writePPM(const std::string& path, const std::vector<Vec3>& pixels, int width, int height) noexcept;

/// Write PNG if stb_image_write.h is available (optional).
bool writePNG(const std::string& path, const std::vector<Vec3>& pixels, int width, int height) noexcept;

} // namespace bh