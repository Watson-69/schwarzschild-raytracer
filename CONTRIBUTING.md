# Contributing to Schwarzschild Raytracer

Thank you for your interest in contributing!

## Development Setup

1. Install a C++17 compiler (MSVC, GCC, or Clang)
2. Install CMake >= 3.16
3. Clone the repository and build:
   ```sh
   cmake --preset release
   cmake --build --preset release
   ```

## Code Style

- Use C++17 features where appropriate
- Prefer `const` and `constexpr`
- Use RAII for resource management
- Write self-documenting code with clear variable names
- Add comments for physics equations and non-obvious logic

## Testing

- Run tests with `ctest` after building
- Add tests for new physics behavior
- Ensure numerical tolerance checks pass

## Pull Requests

1. Fork the repository
2. Create a feature branch
3. Make your changes with clear commit messages
4. Run tests and ensure they pass
5. Submit a PR with a description of the changes

## Reporting Issues

Please include:
- OS and compiler version
- Steps to reproduce
- Expected vs actual behavior
- Screenshots if applicable