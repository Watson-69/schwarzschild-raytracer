// image_io.cpp — PPM/PNG image I/O implementation.
#include "image_io.h"
#include <fstream>
#include <vector>
#include <cmath>

// stb_image_write.h optional - if available, enables PNG
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

namespace bh {

bool savePPM(const std::string& path, const std::vector<unsigned char>& data, int width, int height) noexcept {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out << "P6\n" << width << " " << height << "\n255\n";
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    return out.good();
}

bool writePPM(const std::string& path, const std::vector<Vec3>& pixels, int width, int height) noexcept {
    std::vector<unsigned char> data;
    data.reserve(width * height * 3);
    for (const auto& p : pixels) {
        auto srgb = toSRGB(p);
        data.push_back(srgb[0]);
        data.push_back(srgb[1]);
        data.push_back(srgb[2]);
    }
    return savePPM(path, data, width, height);
}

bool writePNG(const std::string&, const std::vector<Vec3>&, int, int) noexcept {
    // Optional: implement with stb_image_write.h if needed
    return false;
}

} // namespace bh