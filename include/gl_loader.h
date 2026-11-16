// gl_loader.h — Minimal OpenGL 3.3 core function loader via glfwGetProcAddress.
#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <GLFW/glfw3.h>

// We declare function pointers for every GL entry point we use.

extern void (*pglViewport)(int, int, int, int);
#define glViewport pglViewport
extern void (*pglClearColor)(float, float, float, float);
#define glClearColor pglClearColor
extern void (*pglClear)(unsigned int);
#define glClear pglClear
extern unsigned int (*pglCreateShader)(unsigned int);
#define glCreateShader pglCreateShader
extern void (*pglShaderSource)(unsigned int, int, const char *const *, const int *);
#define glShaderSource pglShaderSource
extern void (*pglCompileShader)(unsigned int);
#define glCompileShader pglCompileShader
extern void (*pglGetShaderiv)(unsigned int, unsigned int, int *);
#define glGetShaderiv pglGetShaderiv
extern void (*pglGetShaderInfoLog)(unsigned int, int, int *, char *);
#define glGetShaderInfoLog pglGetShaderInfoLog
extern unsigned int (*pglCreateProgram)(void);
#define glCreateProgram pglCreateProgram
extern void (*pglAttachShader)(unsigned int, unsigned int);
#define glAttachShader pglAttachShader
extern void (*pglLinkProgram)(unsigned int);
#define glLinkProgram pglLinkProgram
extern void (*pglGetProgramiv)(unsigned int, unsigned int, int *);
#define glGetProgramiv pglGetProgramiv
extern void (*pglGetProgramInfoLog)(unsigned int, int, int *, char *);
#define glGetProgramInfoLog pglGetProgramInfoLog
extern void (*pglUseProgram)(unsigned int);
#define glUseProgram pglUseProgram
extern int (*pglGetUniformLocation)(unsigned int, const char *);
#define glGetUniformLocation pglGetUniformLocation
extern void (*pglUniform1i)(int, int);
#define glUniform1i pglUniform1i
extern void (*pglUniform1f)(int, float);
#define glUniform1f pglUniform1f
extern void (*pglUniform2f)(int, float, float);
#define glUniform2f pglUniform2f
extern void (*pglUniform3f)(int, float, float, float);
#define glUniform3f pglUniform3f
extern void (*pglUniform4f)(int, float, float, float, float);
#define glUniform4f pglUniform4f
extern void (*pglUniform1fv)(int, int, const float *);
#define glUniform1fv pglUniform1fv
extern unsigned int (*pglGenVertexArrays)(int, unsigned int *);
#define glGenVertexArrays pglGenVertexArrays
extern void (*pglBindVertexArray)(unsigned int);
#define glBindVertexArray pglBindVertexArray
extern unsigned int (*pglGenBuffers)(int, unsigned int *);
#define glGenBuffers pglGenBuffers
extern void (*pglBindBuffer)(unsigned int, unsigned int);
#define glBindBuffer pglBindBuffer
extern void (*pglBufferData)(unsigned int, long long, const void *, unsigned int);
#define glBufferData pglBufferData
extern void (*pglEnableVertexAttribArray)(unsigned int);
#define glEnableVertexAttribArray pglEnableVertexAttribArray
extern void (*pglVertexAttribPointer)(unsigned int, int, unsigned int, unsigned char, int, const void *);
#define glVertexAttribPointer pglVertexAttribPointer
extern void (*pglDrawArrays)(unsigned int, int, int);
#define glDrawArrays pglDrawArrays
extern void (*pglDeleteShader)(unsigned int);
#define glDeleteShader pglDeleteShader
extern void (*pglDeleteProgram)(unsigned int);
#define glDeleteProgram pglDeleteProgram
extern void (*pglDeleteVertexArrays)(int, const unsigned int *);
#define glDeleteVertexArrays pglDeleteVertexArrays
extern void (*pglDeleteBuffers)(int, const unsigned int *);
#define glDeleteBuffers pglDeleteBuffers
extern const unsigned char * (*pglGetString)(unsigned int);
#define glGetString pglGetString

#undef GL_LOAD_DECL

// Constants
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_FLOAT
#define GL_FLOAT 0x1406
#endif
#ifndef GL_TRIANGLES
#define GL_TRIANGLES 0x0004
#endif
#ifndef GL_COLOR_BUFFER_BIT
#define GL_COLOR_BUFFER_BIT 0x00004000
#endif
#ifndef GL_FALSE
#define GL_FALSE 0
#endif
#ifndef GL_TRUE
#define GL_TRUE 1
#endif
#ifndef GL_VENDOR
#define GL_VENDOR 0x1F00
#endif
#ifndef GL_RENDERER
#define GL_RENDERER 0x1F01
#endif
#ifndef GL_VERSION
#define GL_VERSION 0x1F02
#endif

bool loadGL();
void checkGLError(const char* where);