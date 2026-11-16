// render.cpp
#include "render.h"
#include "config.h"
#include "gl_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace bh {

static const char* VS = R"GLSL(
#version 330 core
layout(location=0) in vec2 aPos;
out vec2 vUV;
void main() {
    vUV = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)GLSL";

static const char* FS = R"GLSL(
#version 330 core
out vec4 fragColor;
in vec2 vUV;

uniform float uMass;
uniform float uRs;
uniform float uDiskInner;
uniform float uDiskOuter;
uniform float uCamR;
uniform float uCamTheta;
uniform float uCamPhi;
uniform float uFov;
uniform float uAspect;
uniform float uStep;
uniform int   uMaxSteps;
uniform float uEscape;

const float PI = 3.14159265359;

float hash3(vec3 p) {
    p = fract(p * 0.3183099 + 0.1);
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

void accel(float r, float th, float dr, float dth, float dph, float E,
           out float arr, out float ath, out float aph) {
    float f = 1.0 - 2.0*uMass/r;
    float sinth = max(sin(th), 1e-6);
    arr = uMass * dr * dr / (r * r * f)
        + r * f * (dth * dth)
        + r * f * sinth * sinth * (dph * dph)
        - uMass * E * E / (r * r * f);
    ath = -2.0 * dr * dth / r + sin(th)*cos(th)*(dph*dph);
    aph = -2.0 * dr * dph / r - 2.0*(cos(th)/sinth)*dth*dph;
}

vec3 starfield(vec3 d) {
    float th = acos(clamp(d.y, -1.0, 1.0));
    float ph = atan(d.z, d.x);
    vec2 uv = vec2(ph/(2.0*PI)+0.5, th/PI);
    vec3 color = vec3(0.0);
    for (int i = 0; i < 3; i++) {
        float scale = 200.0 + float(i)*120.0;
        vec2 g = floor(uv * scale);
        float h = hash3(vec3(g, float(i)));
        if (h > 0.993) {
            float b = (h - 0.993) * 142.857;
            color += vec3(b*0.7, b*0.85, b) * (0.6 + 0.4*h);
        }
    }
    float band = exp(-pow((th - PI*0.5)*3.5, 2.0)) * 0.12;
    color += vec3(0.35, 0.45, 0.75) * band;
    return color;
}

vec3 diskColor(float r, float g) {
    float t = pow(uDiskInner / r, 0.75);
    vec3 cool = vec3(0.85, 0.25, 0.05);
    vec3 hot  = vec3(1.0, 0.92, 0.72);
    vec3 base = mix(cool, hot, clamp(t, 0.0, 1.0));
    float intensity = clamp(pow(max(g, 0.0), 3.0), 0.0, 6.0);
    return base * (0.25 + 0.75 * intensity);
}

void main() {
    vec2 uv = vUV * 2.0 - 1.0;
    uv.x *= uAspect;
    float tanHalf = tan(uFov * 0.5 * PI / 180.0);

    float th = uCamTheta, ph = uCamPhi;
    vec3 e_r = vec3(sin(th)*cos(ph), cos(th), sin(th)*sin(ph));
    vec3 e_th = vec3(cos(th)*cos(ph), -sin(th), cos(th)*sin(ph));
    vec3 e_ph = vec3(-sin(ph), 0.0, cos(ph));

    vec3 fwd = -e_r;
    vec3 right = e_ph;
    vec3 up = e_th;

    vec3 dirWorld = normalize(fwd + uv.x * tanHalf * right + uv.y * tanHalf * up);

    float nr = dot(dirWorld, e_r);
    float nth = dot(dirWorld, e_th);
    float nph = dot(dirWorld, e_ph);

    float r = uCamR;
    float theta = uCamTheta;
    float phi = uCamPhi;
    float f0 = 1.0 - 2.0*uMass/r;
    float dr = sqrt(max(f0, 1e-6)) * nr;
    float dth = nth / r;
    float dph = nph / (r * max(sin(theta), 1e-6));
    float E = sqrt(max(f0, 1e-6));
    float L = r * sin(theta) * nph;

    bool prevAbove = theta > (PI*0.5);

    int kind = 0;
    vec3 finalDir = vec3(0.0);
    float diskR = 0.0, diskG = 1.0;

    for (int i = 0; i < uMaxSteps; i++) {
        float sinth = max(sin(theta), 1e-6);
        if (r <= uRs + 1e-4) { kind = 0; break; }
        if (r >= uEscape) {
            kind = 1;
            float f = 1.0 - 2.0*uMass/r;
            float nr_f = dr / sqrt(max(f, 1e-6));
            float nth_f = r * dth;
            float nph_f = r * sin(theta) * dph;
            float len = sqrt(nr_f*nr_f + nth_f*nth_f + nph_f*nph_f);
            if (len < 1e-6) len = 1.0;
            nr_f /= len; nth_f /= len; nph_f /= len;
            vec3 er = vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
            vec3 et = vec3(cos(theta)*cos(phi), -sin(theta), cos(theta)*sin(phi));
            vec3 ep = vec3(-sin(phi), 0.0, cos(phi));
            finalDir = nr_f*er + nth_f*et + nph_f*ep;
            break;
        }
        bool nowAbove = theta > (PI*0.5);
        if (nowAbove != prevAbove) {
            if (r >= uDiskInner && r <= uDiskOuter) {
                kind = 2;
                diskR = r;
                diskPhi = phi;
                float fcam = 1.0 - 2.0*uMass/uCamR;
                float fdisk = 1.0 - 2.0*uMass/r;
                float Omega = sqrt(uMass / (r*r*r));
                float denom = fdisk - r*r*Omega*Omega;
                if (denom < 1e-6) denom = 1e-6;
                float ut = 1.0 / sqrt(denom);
                float femit = (E - Omega * L) * ut;
                if (abs(femit) < 1e-6) femit = (femit < 0.0) ? -1e-6 : 1e-6;
                float fobs = E / sqrt(max(fcam, 1e-6));
                diskG = fobs / femit;
                break;
            }
        }
        prevAbove = nowAbove;

        // RK4
        float arr1, ath1, aph1;
        accel(r, theta, dr, dth, dph, E, arr1, ath1, aph1);
        float k1r = dr, k1t = dth, k1p = dph;
        float k1dr = arr1, k1dt = ath1, k1dp = aph1;

        float r2 = r + 0.5*uStep*k1r;
        float t2 = theta + 0.5*uStep*k1t;
        float p2 = phi + 0.5*uStep*k1p;
        float dr2 = dr + 0.5*uStep*k1dr;
        float dt2 = dth + 0.5*uStep*k1dt;
        float dp2 = dph + 0.5*uStep*k1dp;
        float arr2, ath2, aph2;
        accel(r2, t2, dr2, dt2, dp2, E, arr2, ath2, aph2);
        float k2r = dr2, k2t = dt2, k2p = dp2;
        float k2dr = arr2, k2dt = ath2, k2dp = aph2;

        float r3 = r + 0.5*uStep*k2r;
        float t3 = theta + 0.5*uStep*k2t;
        float p3 = phi + 0.5*uStep*k2p;
        float dr3 = dr + 0.5*uStep*k2dr;
        float dt3 = dth + 0.5*uStep*k2dt;
        float dp3 = dph + 0.5*uStep*k2dp;
        float arr3, ath3, aph3;
        accel(r3, t3, dr3, dt3, dp3, E, arr3, ath3, aph3);
        float k3r = dr3, k3t = dt3, k3p = dp3;
        float k3dr = arr3, k3dt = ath3, k3dp = aph3;

        float r4 = r + uStep*k3r;
        float t4 = theta + uStep*k3t;
        float p4 = phi + uStep*k3p;
        float dr4 = dr + uStep*k3dr;
        float dt4 = dth + uStep*k3dt;
        float dp4 = dph + uStep*k3dp;
        float arr4, ath4, aph4;
        accel(r4, t4, dr4, dt4, dp4, E, arr4, ath4, aph4);
        float k4r = dr4, k4t = dt4, k4p = dp4;
        float k4dr = arr4, k4dt = ath4, k4dp = aph4;

        r     += uStep/6.0*(k1r + 2.0*k2r + 2.0*k3r + k4r);
        theta += uStep/6.0*(k1t + 2.0*k2t + 2.0*k3t + k4t);
        phi   += uStep/6.0*(k1p + 2.0*k2p + 2.0*k3p + k4p);
        dr    += uStep/6.0*(k1dr + 2.0*k2dr + 2.0*k3dr + k4dr);
        dth   += uStep/6.0*(k1dt + 2.0*k2dt + 2.0*k3dt + k4dt);
        dph   += uStep/6.0*(k1dp + 2.0*k2dp + 2.0*k3dp + k4dp);
    }

    vec3 color;
    if (kind == 0) {
        color = vec3(0.0);
    } else if (kind == 1) {
        color = starfield(finalDir);
    } else {
        color = diskColor(diskR, diskG);
    }
    fragColor = vec4(color, 1.0);
}
)GLSL";

static unsigned int compileShader(unsigned int type, const char* src) {
    unsigned int s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    int ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[4096];
        glGetShaderInfoLog(s, 4096, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << "\n";
    }
    return s;
}

bool Renderer::init() {
    unsigned int vs = compileShader(GL_VERTEX_SHADER, VS);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, FS);
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    int ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[4096];
        glGetProgramInfoLog(program, 4096, nullptr, log);
        std::cerr << "Link error:\n" << log << "\n";
        return false;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    float quad[] = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,
        -1.f, -1.f,
         1.f,  1.f,
        -1.f,  1.f
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindVertexArray(0);

    // cache uniforms
    uMass = glGetUniformLocation(program, "uMass");
    uRs = glGetUniformLocation(program, "uRs");
    uDiskInner = glGetUniformLocation(program, "uDiskInner");
    uDiskOuter = glGetUniformLocation(program, "uDiskOuter");
    uCamR = glGetUniformLocation(program, "uCamR");
    uCamTheta = glGetUniformLocation(program, "uCamTheta");
    uCamPhi = glGetUniformLocation(program, "uCamPhi");
    uFov = glGetUniformLocation(program, "uFov");
    uAspect = glGetUniformLocation(program, "uAspect");
    uStep = glGetUniformLocation(program, "uStep");
    uMaxSteps = glGetUniformLocation(program, "uMaxSteps");
    uEscape = glGetUniformLocation(program, "uEscape");
    return true;
}

void Renderer::render(const Camera& cam, double /*time*/) {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glUniform1f(uMass, (float)MASS);
    glUniform1f(uRs, (float)SCHWARZSCHILD_R);
    glUniform1f(uDiskInner, (float)DISK_INNER);
    glUniform1f(uDiskOuter, (float)DISK_OUTER);
    glUniform1f(uCamR, (float)cam.r);
    glUniform1f(uCamTheta, (float)cam.theta);
    glUniform1f(uCamPhi, (float)cam.phi);
    glUniform1f(uFov, (float)CAMERA_FOV);
    glUniform1f(uAspect, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
    glUniform1f(uStep, (float)STEP_SIZE);
    glUniform1i(uMaxSteps, GPU_MAX_STEPS);
    glUniform1f(uEscape, (float)ESCAPE_RADIUS);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::cleanup() {
    if (program) glDeleteProgram(program);
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
}

} // namespace bh