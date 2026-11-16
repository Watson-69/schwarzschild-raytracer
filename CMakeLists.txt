cmake_minimum_required(VERSION 3.16)
project(BlackHole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

# Core library: math + geodesic physics (no graphics deps)
add_library(bh_core STATIC src/geodesic.cpp)
target_include_directories(bh_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(bh_core PUBLIC cxx_std_17)

# CPU renderer
add_executable(bh_cpu src/render_cpu.cpp)
target_link_libraries(bh_cpu PRIVATE bh_core)

# OpenGL renderer
add_executable(blackhole src/main.cpp src/render.cpp src/gl_loader.cpp)
target_include_directories(blackhole PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(blackhole PRIVATE glfw glm::glm OpenGL::GL)

# Tests
enable_testing()
add_executable(bh_test tests/test_geodesic.cpp)
target_link_libraries(bh_test PRIVATE bh_core)
add_test(NAME geodesic COMMAND bh_test)

# Warnings
if(MSVC)
    target_compile_options(bh_core PRIVATE /W4 /permissive-)
    target_compile_options(bh_cpu PRIVATE /W4 /permissive-)
    target_compile_options(blackhole PRIVATE /W4 /permissive-)
    target_compile_options(bh_test PRIVATE /W4 /permissive-)
else()
    target_compile_options(bh_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(bh_cpu PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(blackhole PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(bh_test PRIVATE -Wall -Wextra -Wpedantic)
endif()