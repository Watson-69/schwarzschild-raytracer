// version.h — Version information.
#pragma once

#define BH_VERSION_MAJOR 1
#define BH_VERSION_MINOR 0
#define BH_VERSION_PATCH 0

#define BH_VERSION_STRING "1.0.0"

namespace bh {
inline constexpr int versionMajor() noexcept { return BH_VERSION_MAJOR; }
inline constexpr int versionMinor() noexcept { return BH_VERSION_MINOR; }
inline constexpr int versionPatch() noexcept { return BH_VERSION_PATCH; }
inline constexpr const char* versionString() noexcept { return BH_VERSION_STRING; }
}