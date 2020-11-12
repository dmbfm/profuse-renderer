#ifndef COFFEE_GL_H
#define COFFEE_GL_H

#include "toolbox_common.h"

#if defined(_WIN32) && !defined(_WIN64)
#define COFFEEAPI __stdcall
#else
#define COFFEEAPI
#endif

#define CGLFUNC(ret, name, ...)                  \
    typedef ret(COFFEEAPI tgl_##name)(__VA_ARGS__); \
    tgl_##name *name;

typedef float GLfloat;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLsizei;
typedef int GLint;
typedef char GLchar;
typedef uint8_t GLubyte;
typedef uint8_t GLboolean;

#ifdef _WIN64
typedef signed long long int CGLintptr;
#else
typedef signed long int CGLintptr;
#endif

typedef CGLintptr GLsizeiptr;
typedef CGLintptr GLintptr;

CGLFUNC(void, glClearColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
CGLFUNC(void, glClear, GLbitfield mask);
CGLFUNC(GLuint, glCreateShader, GLenum shaderType);
CGLFUNC(void, glCompileShader, GLuint shader);
CGLFUNC(void, glDeleteShader, GLuint shader);
CGLFUNC(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
CGLFUNC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params);
CGLFUNC(GLuint, glCreateProgram, void);
CGLFUNC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
CGLFUNC(void, glLinkProgram, GLuint program);
CGLFUNC(void, glAttachShader, GLuint program, GLuint shader);
CGLFUNC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
CGLFUNC(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
CGLFUNC(void, glUseProgram, GLuint program);
CGLFUNC(void, glDrawArrays, GLenum mode, GLint first, GLsizei count);
CGLFUNC(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, void *indices);
CGLFUNC(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
CGLFUNC(void, glBindVertexArray, GLuint vertexArray);
CGLFUNC(void, glVertexAttrib1f, GLuint index, GLfloat v0);
CGLFUNC(const GLubyte *, glGetString, GLenum name);
CGLFUNC(void, glGetIntegerv, GLenum pname, GLint *params);
CGLFUNC(void, glGenBuffers, GLsizei n, GLuint *buffers);
CGLFUNC(void, glBindBuffer, GLenum target, GLuint buffer);
CGLFUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
CGLFUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
CGLFUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
       const void *pointer);
CGLFUNC(void, glEnableVertexAttribArray, GLuint index);
CGLFUNC(void, glDisableVertexAttribArray, GLuint index);
CGLFUNC(void, glEnable, GLenum name);

#define GL_DEPTH_BUFFER_BIT                  0x00000100
#define GL_STENCIL_BUFFER_BIT                0x00000400
#define GL_COLOR_BUFFER_BIT                  0x00004000
#define GL_FALSE                             0
#define GL_TRUE                              1
#define GL_POINTS                            0x0000
#define GL_LINES                             0x0001
#define GL_LINE_LOOP                         0x0002
#define GL_LINE_STRIP                        0x0003
#define GL_TRIANGLES                         0x0004
#define GL_TRIANGLE_STRIP                    0x0005
#define GL_TRIANGLE_FAN                      0x0006
#define GL_QUADS                             0x0007
#define GL_NEVER                             0x0200
#define GL_LESS                              0x0201
#define GL_EQUAL                             0x0202
#define GL_LEQUAL                            0x0203
#define GL_GREATER                           0x0204
#define GL_NOTEQUAL                          0x0205
#define GL_GEQUAL                            0x0206
#define GL_ALWAYS                            0x0207
#define GL_ZERO                              0
#define GL_ONE                               1
#define GL_SRC_COLOR                         0x0300
#define GL_ONE_MINUS_SRC_COLOR               0x0301
#define GL_SRC_ALPHA                         0x0302
#define GL_ONE_MINUS_SRC_ALPHA               0x0303
#define GL_DST_ALPHA                         0x0304
#define GL_ONE_MINUS_DST_ALPHA               0x0305
#define GL_DST_COLOR                         0x0306
#define GL_ONE_MINUS_DST_COLOR               0x0307
#define GL_SRC_ALPHA_SATURATE                0x0308
#define GL_NONE                              0
#define GL_FRONT_LEFT                        0x0400
#define GL_FRONT_RIGHT                       0x0401
#define GL_BACK_LEFT                         0x0402
#define GL_BACK_RIGHT                        0x0403
#define GL_FRONT                             0x0404
#define GL_BACK                              0x0405
#define GL_LEFT                              0x0406
#define GL_RIGHT                             0x0407
#define GL_FRONT_AND_BACK                    0x0408
#define GL_FRAGMENT_SHADER                   0x8B30
#define GL_VERTEX_SHADER                     0x8B31
#define GL_DELETE_STATUS                     0x8B80
#define GL_COMPILE_STATUS                    0x8B81
#define GL_LINK_STATUS                       0x8B82
#define GL_VERSION                           0x1F02
#define GL_CONTEXT_PROFILE_MASK              0x9126
#define GL_CONTEXT_CORE_PROFILE_BIT          0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_MAJOR_VERSION                     0x821B
#define GL_MINOR_VERSION                     0x821C
#define GL_STATIC_DRAW                       0x88E4
#define GL_STATIC_READ                       0x88E5
#define GL_STATIC_COPY                       0x88E6
#define GL_ARRAY_BUFFER                      0x8892
#define GL_ELEMENT_ARRAY_BUFFER              0x8893
#define GL_DONT_CARE                         0x1100
#define GL_FASTEST                           0x1101
#define GL_NICEST                            0x1102
#define GL_BYTE                              0x1400
#define GL_UNSIGNED_BYTE                     0x1401
#define GL_SHORT                             0x1402
#define GL_UNSIGNED_SHORT                    0x1403
#define GL_INT                               0x1404
#define GL_UNSIGNED_INT                      0x1405
#define GL_FLOAT                             0x1406
#define GL_CW                                0x0900
#define GL_CCW                               0x0901
#define GL_POINT_SIZE                        0x0B11
#define GL_POINT_SIZE_RANGE                  0x0B12
#define GL_POINT_SIZE_GRANULARITY            0x0B13
#define GL_LINE_SMOOTH                       0x0B20
#define GL_LINE_WIDTH                        0x0B21
#define GL_LINE_WIDTH_RANGE                  0x0B22
#define GL_LINE_WIDTH_GRANULARITY            0x0B23
#define GL_POLYGON_MODE                      0x0B40
#define GL_POLYGON_SMOOTH                    0x0B41
#define GL_CULL_FACE                         0x0B44
#define GL_CULL_FACE_MODE                    0x0B45
#define GL_FRONT_FACE                        0x0B46
#define GL_DEPTH_RANGE                       0x0B70
#define GL_DEPTH_TEST                        0x0B71
#define GL_DEPTH_WRITEMASK                   0x0B72
#define GL_DEPTH_CLEAR_VALUE                 0x0B73
#define GL_DEPTH_FUNC                        0x0B74
#define GL_STENCIL_TEST                      0x0B90
#define GL_STENCIL_CLEAR_VALUE               0x0B91
#define GL_STENCIL_FUNC                      0x0B92
#define GL_STENCIL_VALUE_MASK                0x0B93
#define GL_STENCIL_FAIL                      0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL           0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS           0x0B96
#define GL_STENCIL_REF                       0x0B97
#define GL_STENCIL_WRITEMASK                 0x0B98
#define GL_VIEWPORT                          0x0BA2
#define GL_DITHER                            0x0BD0
#define GL_BLEND_DST                         0x0BE0
#define GL_BLEND_SRC                         0x0BE1
#define GL_BLEND                             0x0BE2
#define GL_LOGIC_OP_MODE                     0x0BF0

#endif
