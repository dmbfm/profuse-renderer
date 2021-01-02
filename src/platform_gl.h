#ifndef _PLATFORM_GL_H
#define _PLATFORM_GL_H

#include "common.h"

#define GLFUNC(ret, name, ...)                    \
    typedef ret(CSTDCALL gl_##name)(__VA_ARGS__); \
    extern gl_##name *name;

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

// clang-format off
GLFUNC(void, glClearColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
GLFUNC(void, glClear, GLbitfield mask);
GLFUNC(GLuint, glCreateShader, GLenum shaderType);
GLFUNC(void, glCompileShader, GLuint shader);
GLFUNC(void, glDeleteShader, GLuint shader);
GLFUNC(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLFUNC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params);
GLFUNC(GLuint, glCreateProgram, void);
GLFUNC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
GLFUNC(void, glLinkProgram, GLuint program);
GLFUNC(void, glDeleteProgram, GLuint program);
GLFUNC(void, glAttachShader, GLuint program, GLuint shader);
GLFUNC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
GLFUNC(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLFUNC(void, glUseProgram, GLuint program);
GLFUNC(void, glDrawArrays, GLenum mode, GLint first, GLsizei count);
GLFUNC(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, void *indices);
GLFUNC(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
GLFUNC(void, glBindVertexArray, GLuint vertexArray);
GLFUNC(void, glVertexAttrib1f, GLuint index, GLfloat v0);
GLFUNC(const GLubyte *, glGetString, GLenum name);
GLFUNC(void, glGetIntegerv, GLenum pname, GLint *params);
GLFUNC(void, glGenBuffers, GLsizei n, GLuint *buffers);
GLFUNC(void, glBindBuffer, GLenum target, GLuint buffer);
GLFUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLFUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GLFUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
   const void *pointer);
GLFUNC(void, glEnableVertexAttribArray, GLuint index);
GLFUNC(void, glDisableVertexAttribArray, GLuint index);
GLFUNC(void, glEnable, GLenum name);
GLFUNC(GLint, glGetUniformLocation, GLuint program, const GLchar *pname);
GLFUNC(void, glUniformMatrix4fv, GLuint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLFUNC(void, glUniform3fv, GLuint location, const GLfloat *value);
GLFUNC(void, glUniform1i, GLuint location, GLint v0);
GLFUNC(void, glGenTextures, GLsizei n, GLuint *textures);
GLFUNC(void, glActiveTexture, GLenum texture);
GLFUNC(void, glBindTexture, GLenum target, GLuint texture);
GLFUNC(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
   GLenum format, GLenum type, const void *pixels);
GLFUNC(void, glTexParameteri, GLenum target, GLenum pname, GLint param);
GLFUNC(void, glPixelStorei, GLenum pname, GLint param);
GLFUNC(void, glBlendFunc, GLenum sfactor, GLenum dfactor);
GLFUNC(void, glGenerateMipmap, GLenum target);
// clang-format on

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
#define GL_TEXTURE_1D                        0x0DE0
#define GL_TEXTURE_2D                        0x0DE1
#define GL_TEXTURE_WIDTH                     0x1000
#define GL_TEXTURE_HEIGHT                    0x1001
#define GL_TEXTURE_BORDER_COLOR              0x1004
#define GL_NEAREST                           0x2600
#define GL_LINEAR                            0x2601
#define GL_NEAREST_MIPMAP_NEAREST            0x2700
#define GL_LINEAR_MIPMAP_NEAREST             0x2701
#define GL_NEAREST_MIPMAP_LINEAR             0x2702
#define GL_LINEAR_MIPMAP_LINEAR              0x2703
#define GL_TEXTURE_MAG_FILTER                0x2800
#define GL_TEXTURE_MIN_FILTER                0x2801
#define GL_TEXTURE_WRAP_S                    0x2802
#define GL_TEXTURE_WRAP_T                    0x2803
#define GL_REPEAT                            0x2901
#define GL_TEXTURE                           0x1702
#define GL_TEXTURE0                          0x84C0
#define GL_TEXTURE1                          0x84C1
#define GL_TEXTURE2                          0x84C2
#define GL_TEXTURE3                          0x84C3
#define GL_TEXTURE4                          0x84C4
#define GL_TEXTURE5                          0x84C5
#define GL_TEXTURE6                          0x84C6
#define GL_TEXTURE7                          0x84C7
#define GL_TEXTURE8                          0x84C8
#define GL_TEXTURE9                          0x84C9
#define GL_TEXTURE10                         0x84CA
#define GL_TEXTURE11                         0x84CB
#define GL_TEXTURE12                         0x84CC
#define GL_TEXTURE13                         0x84CD
#define GL_TEXTURE14                         0x84CE
#define GL_TEXTURE15                         0x84CF
#define GL_TEXTURE16                         0x84D0
#define GL_TEXTURE17                         0x84D1
#define GL_TEXTURE18                         0x84D2
#define GL_TEXTURE19                         0x84D3
#define GL_TEXTURE20                         0x84D4
#define GL_TEXTURE21                         0x84D5
#define GL_TEXTURE22                         0x84D6
#define GL_TEXTURE23                         0x84D7
#define GL_TEXTURE24                         0x84D8
#define GL_TEXTURE25                         0x84D9
#define GL_TEXTURE26                         0x84DA
#define GL_TEXTURE27                         0x84DB
#define GL_TEXTURE28                         0x84DC
#define GL_TEXTURE29                         0x84DD
#define GL_TEXTURE30                         0x84DE
#define GL_TEXTURE31                         0x84DF
#define GL_ACTIVE_TEXTURE                    0x84E0
#define GL_MULTISAMPLE                       0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE          0x809E
#define GL_SAMPLE_ALPHA_TO_ONE               0x809F
#define GL_SAMPLE_COVERAGE                   0x80A0
#define GL_SAMPLE_BUFFERS                    0x80A8
#define GL_SAMPLES                           0x80A9
#define GL_SAMPLE_COVERAGE_VALUE             0x80AA
#define GL_SAMPLE_COVERAGE_INVERT            0x80AB
#define GL_TEXTURE_CUBE_MAP                  0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP          0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X       0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X       0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y       0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y       0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z       0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z       0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP            0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE         0x851C
#define GL_COMPRESSED_RGB                    0x84ED
#define GL_COMPRESSED_RGBA                   0x84EE
#define GL_TEXTURE_COMPRESSION_HINT          0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE     0x86A0
#define GL_TEXTURE_COMPRESSED                0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS    0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS        0x86A3
#define GL_CLAMP_TO_BORDER                   0x812D
#define GL_CLIENT_ACTIVE_TEXTURE             0x84E1
#define GL_MAX_TEXTURE_UNITS                 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX        0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX       0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX          0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX            0x84E6
#define GL_MULTISAMPLE_BIT                   0x20000000
#define GL_NORMAL_MAP                        0x8511
#define GL_REFLECTION_MAP                    0x8512
#define GL_COMPRESSED_ALPHA                  0x84E9
#define GL_COMPRESSED_LUMINANCE              0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA        0x84EB
#define GL_COMPRESSED_INTENSITY              0x84EC
#define GL_COMBINE                           0x8570
#define GL_COMBINE_RGB                       0x8571
#define GL_COMBINE_ALPHA                     0x8572
#define GL_SOURCE0_RGB                       0x8580
#define GL_SOURCE1_RGB                       0x8581
#define GL_SOURCE2_RGB                       0x8582
#define GL_SOURCE0_ALPHA                     0x8588
#define GL_SOURCE1_ALPHA                     0x8589
#define GL_SOURCE2_ALPHA                     0x858A
#define GL_OPERAND0_RGB                      0x8590
#define GL_OPERAND1_RGB                      0x8591
#define GL_OPERAND2_RGB                      0x8592
#define GL_OPERAND0_ALPHA                    0x8598
#define GL_OPERAND1_ALPHA                    0x8599
#define GL_OPERAND2_ALPHA                    0x859A
#define GL_RGB_SCALE                         0x8573
#define GL_ADD_SIGNED                        0x8574
#define GL_INTERPOLATE                       0x8575
#define GL_SUBTRACT                          0x84E7
#define GL_CONSTANT                          0x8576
#define GL_PRIMARY_COLOR                     0x8577
#define GL_PREVIOUS                          0x8578
#define GL_DOT3_RGB                          0x86AE
#define GL_DOT3_RGBA                         0x86AF
#define GL_BLEND_DST_RGB                     0x80C8
#define GL_BLEND_SRC_RGB                     0x80C9
#define GL_BLEND_DST_ALPHA                   0x80CA
#define GL_BLEND_SRC_ALPHA                   0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE         0x8128
#define GL_DEPTH_COMPONENT16                 0x81A5
#define GL_DEPTH_COMPONENT24                 0x81A6
#define GL_DEPTH_COMPONENT32                 0x81A7
#define GL_MIRRORED_REPEAT                   0x8370
#define GL_MAX_TEXTURE_LOD_BIAS              0x84FD
#define GL_TEXTURE_LOD_BIAS                  0x8501
#define GL_INCR_WRAP                         0x8507
#define GL_DECR_WRAP                         0x8508
#define GL_TEXTURE_DEPTH_SIZE                0x884A
#define GL_TEXTURE_COMPARE_MODE              0x884C
#define GL_TEXTURE_COMPARE_FUNC              0x884D
#define GL_COLOR                             0x1800
#define GL_DEPTH                             0x1801
#define GL_STENCIL                           0x1802
#define GL_STENCIL_INDEX                     0x1901
#define GL_DEPTH_COMPONENT                   0x1902
#define GL_RED                               0x1903
#define GL_GREEN                             0x1904
#define GL_BLUE                              0x1905
#define GL_ALPHA                             0x1906
#define GL_RGB                               0x1907
#define GL_RGBA                              0x1908
#define GL_LUMINANCE                         0x1909
#define GL_LUMINANCE_ALPHA                   0x190A
#define GL_UNSIGNED_BYTE_3_3_2               0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4            0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1            0x8034
#define GL_UNSIGNED_INT_8_8_8_8              0x8035
#define GL_UNSIGNED_INT_10_10_10_2           0x8036
#define GL_CLAMP_TO_EDGE                     0x812F
#define GL_UNPACK_FLIP_Y_WEBGL               0x9240
#define GL_INFO_LOG_LENGTH                   0x8B84

#define WGL_NUMBER_PIXEL_FORMATS_ARB              0x2000
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_DRAW_TO_BITMAP_ARB                    0x2002
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_NEED_PALETTE_ARB                      0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB               0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB                0x2006
#define WGL_SWAP_METHOD_ARB                       0x2007
#define WGL_NUMBER_OVERLAYS_ARB                   0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                  0x2009
#define WGL_TRANSPARENT_ARB                       0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB             0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB           0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB            0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB           0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB           0x203B
#define WGL_SHARE_DEPTH_ARB                       0x200C
#define WGL_SHARE_STENCIL_ARB                     0x200D
#define WGL_SHARE_ACCUM_ARB                       0x200E
#define WGL_SUPPORT_GDI_ARB                       0x200F
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_STEREO_ARB                            0x2012
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_RED_BITS_ARB                          0x2015
#define WGL_RED_SHIFT_ARB                         0x2016
#define WGL_GREEN_BITS_ARB                        0x2017
#define WGL_GREEN_SHIFT_ARB                       0x2018
#define WGL_BLUE_BITS_ARB                         0x2019
#define WGL_BLUE_SHIFT_ARB                        0x201A
#define WGL_ALPHA_BITS_ARB                        0x201B
#define WGL_ALPHA_SHIFT_ARB                       0x201C
#define WGL_ACCUM_BITS_ARB                        0x201D
#define WGL_ACCUM_RED_BITS_ARB                    0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                  0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                   0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                  0x2021
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_AUX_BUFFERS_ARB                       0x2024
#define WGL_NO_ACCELERATION_ARB                   0x2025
#define WGL_GENERIC_ACCELERATION_ARB              0x2026
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_SWAP_EXCHANGE_ARB                     0x2028
#define WGL_SWAP_COPY_ARB                         0x2029
#define WGL_SWAP_UNDEFINED_ARB                    0x202A
#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_TYPE_COLORINDEX_ARB                   0x202C
#define WGL_SAMPLE_BUFFERS_ARB                    0x2041
#define WGL_SAMPLES_ARB                           0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB                 0x2095
#define ERROR_INVALID_PROFILE_ARB                 0x2096

#endif /* _PLATFORM_GL_H */
