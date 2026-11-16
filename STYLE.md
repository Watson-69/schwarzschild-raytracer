# Coding Style Guide

## General
- C++17 standard
- 4 spaces indentation, no tabs
- Line length: 100 characters max
- LF line endings

## Naming
- Types: PascalCase (e.g., `TraceResult`)
- Functions: camelCase (e.g., `traceRay`)
- Variables: snake_case (e.g., `max_steps`)
- Constants: UPPER_SNAKE_CASE (e.g., `MAX_STEPS`)
- Namespaces: lower_case (e.g., `bh`)

## Formatting
- Braces on same line for functions, classes, control structures
- Space after keywords: `if (cond)`, `for (int i = 0; ...)`
- No space after function name: `func(arg)`
- Pointer/reference binds to type: `Vec3& a`

## Includes
- Standard library first, then project headers
- Alphabetical within groups
- Use `#pragma once` for headers

## Comments
- Doxygen-style for public API
- Explain physics equations in comments
- No redundant comments

## Modern C++
- Prefer `constexpr`/`const` over macros
- Use `noexcept` where appropriate
- Prefer RAII over manual cleanup
- Use standard library algorithms