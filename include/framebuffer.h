// framebuffer.h — Offscreen framebuffer with multiple attachments.
#pragma once

#include "gl_loader.h"
#include <cstdint>

namespace bh {

struct Framebuffer {
    uint32_t fbo = 0;
    uint32_t colorTex = 0;
    uint32_t depthTex = 0;
    int width = 0, height = 0;

    Framebuffer() = default;
    ~Framebuffer() { cleanup(); }

    bool create(int w, int h) {
        cleanup();
        width = w; height = h;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Color attachment
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

        // Depth attachment
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            cleanup(); return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }

    void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
    void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void resize(int w, int h) {
        if (w == width && h == height) return;
        cleanup();
        create(w, h);
    }

    void cleanup() {
        if (colorTex) { glDeleteTextures(1, &colorTex); colorTex = 0; }
        if (depthTex) { glDeleteTextures(1, &depthTex); depthTex = 0; }
        if (fbo) { glDeleteFramebuffers(1, &fbo); fbo = 0; }
        width = height = 0;
    }

    uint32_t colorTexture() const { return colorTex; }
};

} // namespace bh