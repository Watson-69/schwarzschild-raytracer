# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Schwarzschild null geodesic integration with RK4
- Interactive OpenGL 3.3 visualiser
- CPU fallback renderer writing PPM/PNG
- Accretion disk with gravitational redshift and Doppler beaming
- Procedural starfield background
- Unit tests for geodesic integrator
- CPU benchmark program
- GitHub Actions CI for Linux and Windows
- CMake presets for debug/release
- Modern CMake with target-based build
- Separate GLSL shader files
- Comprehensive documentation

### Changed
- Refactored rendering logic into shared module (render_common.h)
- Expanded configuration with named constants and units
- Improved test coverage with tolerance checks
- Modernized CMake with target-based commands and warnings

### Fixed
- Unused parameter warning in gl_loader.cpp
- Enum class for trace result kind
- Removed unused fields and dead code