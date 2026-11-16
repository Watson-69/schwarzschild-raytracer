// gl_loader.cpp
#include "gl_loader.h"
#include <iostream>

#define GL_DEFINE(ret, name, ...) ret (*p##name)(__VA_ARGS__) = nullptr;

GL_DEFINE(void, glViewport, int, int, int, int)
GL_DEFINE(void, glClearColor, float, float, float, float)
GL_DEFINE(void, glClear, unsigned int)
GL_DEFINE(unsigned int, glCreateShader, unsigned int)
GL_DEFINE(void, glShaderSource, unsigned int, int, const char *const *, const int *)
GL_DEFINE(void, glCompileShader, unsigned int)
GL_DEFINE(void, glGetShaderiv, unsigned int, unsigned int, int *)
GL_DEFINE(void, glGetShaderInfoLog, unsigned int, int, int *, char *)
GL_DEFINE(unsigned int, glCreateProgram, void)
GL_DEFINE(void, glAttachShader, unsigned int, unsigned int)
GL_DEFINE(void, glLinkProgram, unsigned int)
GL_DEFINE(void, glGetProgramiv, unsigned int, unsigned int, int *)
GL_DEFINE(void, glGetProgramInfoLog, unsigned int, int, int *, char *)
GL_DEFINE(void, glUseProgram, unsigned int)
GL_DEFINE(int, glGetUniformLocation, unsigned int, const char *)
GL_DEFINE(void, glUniform1i, int, int)
GL_DEFINE(void, glUniform1f, int, float)
GL_DEFINE(void, glUniform2f, int, float, float)
GL_DEFINE(void, glUniform3f, int, float, float, float)
GL_DEFINE(void, glUniform4f, int, float, float, float, float)
GL_DEFINE(void, glUniform1fv, int, int, const float *)
GL_DEFINE(unsigned int, glGenVertexArrays, int, unsigned int *)
GL_DEFINE(void, glBindVertexArray, unsigned int)
GL_DEFINE(unsigned int, glGenBuffers, int, unsigned int *)
GL_DEFINE(void, glBindBuffer, unsigned int, unsigned int)
GL_DEFINE(void, glBufferData, unsigned int, long long, const void *, unsigned int)
GL_DEFINE(void, glEnableVertexAttribArray, unsigned int)
GL_DEFINE(void, glVertexAttribPointer, unsigned int, int, unsigned int, unsigned char, int, const void *)
GL_DEFINE(void, glDrawArrays, unsigned int, int, int)
GL_DEFINE(void, glDeleteShader, unsigned int)
GL_DEFINE(void, glDeleteProgram, unsigned int)
GL_DEFINE(void, glDeleteVertexArrays, int, const unsigned int *)
GL_DEFINE(void, glDeleteBuffers, int, const unsigned int *)
GL_DEFINE(const unsigned char *, glGetString, unsigned int)

#undef GL_DEFINE

#define LOAD(name) p##name = reinterpret_cast<decltype(p##name)>(glfwGetProcAddress(#name)); \
    if (!p##name) { std::cerr << "Failed to load " << #name << "\n"; ok = false; }

bool loadGL() {
    bool ok = true;
    LOAD(glViewport);
    LOAD(glClearColor);
    LOAD(glClear);
    LOAD(glCreateShader);
    LOAD(glShaderSource);
    LOAD(glCompileShader);
    LOAD(glGetShaderiv);
    LOAD(glGetShaderInfoLog);
    LOAD(glCreateProgram);
    LOAD(glAttachShader);
    LOAD(glLinkProgram);
    LOAD(glGetProgramiv);
    LOAD(glGetProgramInfoLog);
    LOAD(glUseProgram);
    LOAD(glGetUniformLocation);
    LOAD(glUniform1i);
    LOAD(glUniform1f);
    LOAD(glUniform2f);
    LOAD(glUniform3f);
    LOAD(glUniform4f);
    LOAD(glUniform1fv);
    LOAD(glGenVertexArrays);
    LOAD(glBindVertexArray);
    LOAD(glGenBuffers);
    LOAD(glBindBuffer);
    LOAD(glBufferData);
    LOAD(glEnableVertexAttribArray);
    LOAD(glVertexAttribPointer);
    LOAD(glDrawArrays);
    LOAD(glDeleteShader);
    LOAD(glDeleteProgram);
    LOAD(glDeleteVertexArrays);
    LOAD(glDeleteBuffers);
    LOAD(glGetString);
    return ok;
}

void checkGLError(const char* where) {
    (void)where;
}