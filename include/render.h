// render.h — OpenGL rendering of the black hole via fullscreen ray-marching shader.
#pragma once

#include "config.h"
#include "gl_loader.h"
#include <GLFW/glfw3.h>

namespace bh {
struct Camera {
    double r = CAMERA_DISTANCE;
    double theta = 1.2;
    double phi = 0.0;
};

class Renderer {
public:
    bool init();
    void render(const Camera& cam, double time);
    void cleanup();
    int program = 0;
    unsigned int vao = 0, vbo = 0;
    // uniform locations
    int uMass=0,uRs=0,uDiskInner=0,uDiskOuter=0,uCamR=0,uCamTheta=0,uCamPhi=0;
    int uFov=0,uAspect=0,uStep=0,uMaxSteps=0,uEscape=0;
};
}
