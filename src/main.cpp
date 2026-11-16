// main.cpp
#include "config.h"
#include "render.h"
#include "gl_loader.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace bh;

static Camera gCam;
static bool gDragging = false;
static double gLastX = 0, gLastY = 0;

static void mouseButtonCallback(GLFWwindow* w, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            gDragging = true;
            glfwGetCursorPos(w, &gLastX, &gLastY);
        } else {
            gDragging = false;
        }
    }
}

static void cursorPosCallback(GLFWwindow*, double x, double y) {
    if (gDragging) {
        double dx = x - gLastX;
        double dy = y - gLastY;
        gCam.orbit(dx, dy);
        gLastX = x; gLastY = y;
    }
}

static void scrollCallback(GLFWwindow*, double, double y) {
    gCam.zoom(y);
}

int main() {
    if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Black Hole", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!loadGL()) { std::cerr << "Failed to load GL functions\n"; return -1; }

    Renderer renderer;
    if (!renderer.init()) { std::cerr << "Failed to init renderer\n"; return -1; }

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    double lastTime = glfwGetTime();
    int frames = 0;
    double fpsTimer = lastTime;

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        ++frames;
        if (now - fpsTimer >= 0.5) {
            double fps = frames / (now - fpsTimer);
            glfwSetWindowTitle(window, ("Black Hole - " + std::to_string((int)fps) + " FPS").c_str());
            frames = 0; fpsTimer = now;
        }

        renderer.render(gCam, now);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}