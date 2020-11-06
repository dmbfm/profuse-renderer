#if !defined(_TG_GL_H)
#define _TG_GL_H

#if defined(_WIN32) && !defined(_WIN64)
#define TGLAPI __stdcall
#else
#define TGLAPI
#endif

#include <stdint.h>

// TODO: Remove these externs which are only here to make clangd happy.
#ifndef tg_printf
extern int printf(const char *, ...);
#define tg_printf printf
#endif

#ifndef tg_assert
#define tg_assert(exp) \
    if (!(exp)) {      \
        abort();       \
    }
#endif

#define TGLFUNC(ret, name, ...)                  \
    typedef ret(TGLAPI tgl_##name)(__VA_ARGS__); \
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
typedef signed long long int TGLintptr;
#else
typedef signed long int TGLintptr;
#endif

typedef TGLintptr GLsizeiptr;
typedef TGLintptr GLintptr;

TGLFUNC(void, glClearColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
TGLFUNC(void, glClear, GLbitfield mask);
TGLFUNC(GLuint, glCreateShader, GLenum shaderType);
TGLFUNC(void, glCompileShader, GLuint shader);
TGLFUNC(void, glDeleteShader, GLuint shader);
TGLFUNC(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
TGLFUNC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params);
TGLFUNC(GLuint, glCreateProgram, void);
TGLFUNC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
TGLFUNC(void, glLinkProgram, GLuint program);
TGLFUNC(void, glAttachShader, GLuint program, GLuint shader);
TGLFUNC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
TGLFUNC(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
TGLFUNC(void, glUseProgram, GLuint program);
TGLFUNC(void, glDrawArrays, GLenum mode, GLint first, GLsizei count);
TGLFUNC(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, void *indices);
TGLFUNC(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
TGLFUNC(void, glBindVertexArray, GLuint vertexArray);
TGLFUNC(void, glVertexAttrib1f, GLuint index, GLfloat v0);
TGLFUNC(const GLubyte *, glGetString, GLenum name);
TGLFUNC(void, glGetIntegerv, GLenum pname, GLint *params);
TGLFUNC(void, glGenBuffers, GLsizei n, GLuint *buffers);
TGLFUNC(void, glBindBuffer, GLenum target, GLuint buffer);
TGLFUNC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
TGLFUNC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
TGLFUNC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
        const void *pointer);
TGLFUNC(void, glEnableVertexAttribArray, GLuint index);
TGLFUNC(void, glDisableVertexAttribArray, GLuint index);
TGLFUNC(void, glEnable, GLenum name);

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

typedef enum tgl_ContextProfileType
{
    TGL_CONTEXT_PROFILE_CORE,
    TGL_CONTEXT_PROFILE_COMPABILITY
} tgl_ContextProfileType;

typedef struct tgl_ContextInfo
{
    tgl_ContextProfileType profile_type;
    int version_major;
    int version_minor;
} tgl_ContextInfo;

void tgl_context_info_get(tgl_ContextInfo *info)
{
    int profilemaskv = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profilemaskv);

    if (profilemaskv == GL_CONTEXT_CORE_PROFILE_BIT) {
        info->profile_type = TGL_CONTEXT_PROFILE_CORE;
    } else {
        info->profile_type = TGL_CONTEXT_PROFILE_COMPABILITY;
    }

    glGetIntegerv(GL_MAJOR_VERSION, &info->version_major);
    glGetIntegerv(GL_MINOR_VERSION, &info->version_minor);
}

void tgl_context_info_printf(tgl_ContextInfo *info)
{
    tg_printf("tgl_ContextInfo: ");
    tg_printf("%d.%d ", info->version_major, info->version_minor);
    if (info->profile_type == TGL_CONTEXT_PROFILE_CORE) {
        tg_printf("(Core Profile)\n");
    } else {
        tg_printf("(Compability Profile)\n");
    }
}

GLuint DEBUG_tgl_create_program(char *vshader, char *fshader)
{
    GLuint vertexshader, fragshader, program;

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragshader   = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexshader, 1, (const char **)&vshader, 0);
    glCompileShader(vertexshader);

    GLint cs = 0;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &cs);
    if (cs == GL_FALSE) {
        char buf[256];
        glGetShaderInfoLog(vertexshader, 256, 0, buf);
        tg_printf("VSHADER ERROR: %s\n", buf);

        glDeleteShader(vertexshader);
        return 0;
    }

    glShaderSource(fragshader, 1, (const char **)&fshader, 0);
    glCompileShader(fragshader);

    cs = 0;
    glGetShaderiv(fragshader, GL_COMPILE_STATUS, &cs);
    if (cs == GL_FALSE) {
        char buf[256];
        glGetShaderInfoLog(fragshader, 256, 0, buf);
        tg_printf("FSHADER ERROR: %s\n", buf);

        glDeleteShader(fragshader);
        return 0;
    }

    program = glCreateProgram();

    glAttachShader(program, vertexshader);
    glAttachShader(program, fragshader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &cs);

    if (cs == GL_FALSE) {
        char buf[256];
        glGetProgramInfoLog(program, 256, 0, buf);
        tg_printf("ERROR: %s\n", buf);
        glDeleteShader(vertexshader);
        glDeleteShader(fragshader);
        // glDeleteProgram(program);
    }

    return program;
}

#ifdef _WIN32
#include <Windows.h>
#include <wingdi.h>

TGLFUNC(HGLRC, tgl_wglCreateContext, HDC Arg1);
TGLFUNC(PROC, tgl_wglGetProcAddress, LPCSTR Arg1);
TGLFUNC(BOOL, tgl_wglDeleteContext, HGLRC Arg);
TGLFUNC(BOOL, tgl_wglMakeCurrent, HDC Arg1, HGLRC Arg2);
TGLFUNC(BOOL, wglChoosePixelFormatARB, HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
        int *piFormats, UINT *nNumFormats);
TGLFUNC(HGLRC, wglCreateContextAttribsARB, HDC hDC, HGLRC hshareContext, const int *attribList);

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

HGLRC tgl__win32_glcontext;
tgl_ContextInfo tgl__win32_context_info;

void tgl__win32__init_wgl()
{

    HMODULE tgl__win32_opengl;

    tgl__win32_opengl = LoadLibraryA("opengl32.dll");

    tgl_wglGetProcAddress = (tgl_tgl_wglGetProcAddress *)GetProcAddress(tgl__win32_opengl, "wglGetProcAddress");
    tgl_wglMakeCurrent    = (tgl_tgl_wglMakeCurrent *)GetProcAddress(tgl__win32_opengl, "wglMakeCurrent");
    tgl_wglCreateContext  = (tgl_tgl_wglCreateContext *)GetProcAddress(tgl__win32_opengl, "wglCreateContext");
    tgl_wglDeleteContext  = (tgl_tgl_wglDeleteContext *)GetProcAddress(tgl__win32_opengl, "wglDeleteContext");

    tg_assert(tgl_wglGetProcAddress);
    tg_assert(tgl_wglMakeCurrent);
    tg_assert(tgl_wglCreateContext);
    tg_assert(tgl_wglDeleteContext);

    if (tgl__win32_opengl) {
        FreeLibrary(tgl__win32_opengl);
    }
}

void *tgl__get_proc_adress(const char *name)
{
    void *p = (void *)tgl_wglGetProcAddress(name);
    if (p == 0 || (p == (void *)0x1) || (p == (void *)0x2) || (p == (void *)0x3) || (p == (void *)-1)) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p              = (void *)GetProcAddress(module, name);
    }

    return p;
}

#define TGL_GETPROC(name) name = (tgl_##name *)tgl__get_proc_adress(#name)
void DEBUG_tgl_init_function_pointers()
{
    TGL_GETPROC(glClear);
    TGL_GETPROC(glClearColor);
    TGL_GETPROC(glClearColor);
    TGL_GETPROC(glClear);
    TGL_GETPROC(glCreateShader);
    TGL_GETPROC(glCompileShader);
    TGL_GETPROC(glDeleteShader);
    TGL_GETPROC(glGetShaderInfoLog);
    TGL_GETPROC(glGetShaderiv);
    TGL_GETPROC(glCreateProgram);
    TGL_GETPROC(glShaderSource);
    TGL_GETPROC(glLinkProgram);
    TGL_GETPROC(glAttachShader);
    TGL_GETPROC(glGetProgramiv);
    TGL_GETPROC(glGetProgramInfoLog);
    TGL_GETPROC(glUseProgram);
    TGL_GETPROC(glDrawArrays);
    TGL_GETPROC(glDrawElements);
    TGL_GETPROC(glGenVertexArrays);
    TGL_GETPROC(glBindVertexArray);
    TGL_GETPROC(glVertexAttrib1f);
    TGL_GETPROC(glGetString);
    TGL_GETPROC(glGetIntegerv);
    TGL_GETPROC(glGenBuffers);
    TGL_GETPROC(glBindBuffer);
    TGL_GETPROC(glBufferData);
    TGL_GETPROC(glBufferSubData);
    TGL_GETPROC(glVertexAttribPointer);
    TGL_GETPROC(glEnableVertexAttribArray);
    TGL_GETPROC(glDisableVertexAttribArray);
    TGL_GETPROC(glEnable);

    tg_assert(glClear);
    tg_assert(glClearColor);
    tg_assert(glClearColor);
    tg_assert(glClear);
    tg_assert(glCreateShader);
    tg_assert(glCompileShader);
    tg_assert(glDeleteShader);
    tg_assert(glGetShaderInfoLog);
    tg_assert(glGetShaderiv);
    tg_assert(glCreateProgram);
    tg_assert(glShaderSource);
    tg_assert(glLinkProgram);
    tg_assert(glAttachShader);
    tg_assert(glGetProgramiv);
    tg_assert(glGetProgramInfoLog);
    tg_assert(glUseProgram);
    tg_assert(glDrawArrays);
    tg_assert(glDrawElements);
    tg_assert(glGenVertexArrays);
    tg_assert(glBindVertexArray);
    tg_assert(glVertexAttrib1f);
    tg_assert(glGetString);
    tg_assert(glGetIntegerv);
    tg_assert(glGenBuffers);
    tg_assert(glBindBuffer);
    tg_assert(glBufferData);
    tg_assert(glBufferSubData);
    tg_assert(glVertexAttribPointer);
    tg_assert(glEnableVertexAttribArray);
    tg_assert(glDisableVertexAttribArray);
    tg_assert(glEnable);
}

int tgl__win32_init_wgl_extensions()
{
    // Create a temporary window
    WNDCLASS wc = { .style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
                    .lpfnWndProc   = DefWindowProc,
                    .hInstance     = GetModuleHandleA(0),
                    .lpszClassName = "TGPTEMPWINDOW" };

    RegisterClass(&wc);

    HWND window = CreateWindow(wc.lpszClassName, "TGPTEMPWINDOW", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, 0, 0, wc.hInstance, 0);

    if (!window) {
        return 0;
    }

    HDC dummy_dc = GetDC(window);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
                                  1,
                                  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                                  PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
                                  32,            // Colordepth of the framebuffer.
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  24, // Number of bits for the depthbuffer
                                  8,  // Number of bits for the stencilbuffer
                                  0,  // Number of Aux buffers in the framebuffer.
                                  PFD_MAIN_PLANE,
                                  0,
                                  0,
                                  0,
                                  0 };

    int cpf = ChoosePixelFormat(dummy_dc, &pfd);
    SetPixelFormat(dummy_dc, cpf, &pfd);

    HGLRC glcontext = tgl_wglCreateContext(dummy_dc);

    if (!glcontext) {
        return -1;
    }

    tgl_wglMakeCurrent(dummy_dc, glcontext);

    wglChoosePixelFormatARB    = (tgl_wglChoosePixelFormatARB *)tgl_wglGetProcAddress("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (tgl_wglCreateContextAttribsARB *)tgl_wglGetProcAddress("wglCreateContextAttribsARB");

    tg_assert(wglChoosePixelFormatARB);
    tg_assert(wglCreateContextAttribsARB);

    tgl_wglMakeCurrent(dummy_dc, 0);
    tgl_wglDeleteContext(glcontext);
    ReleaseDC(window, dummy_dc);
    DestroyWindow(window);

    return 1;
}

void DEBUG_tgl_init(HWND window_handle)
{
    tgl__win32__init_wgl();

    tgl__win32_init_wgl_extensions();

    HDC hdc = GetDC(window_handle);

    unsigned int pixelformatcount = 0;

    int pixel_attribis[] = { WGL_DRAW_TO_WINDOW_ARB,
                             GL_TRUE,
                             WGL_SUPPORT_OPENGL_ARB,
                             GL_TRUE,
                             WGL_ACCELERATION_ARB,
                             WGL_FULL_ACCELERATION_ARB,
                             WGL_DOUBLE_BUFFER_ARB,
                             GL_TRUE,
                             WGL_PIXEL_TYPE_ARB,
                             WGL_TYPE_RGBA_ARB,
                             WGL_COLOR_BITS_ARB,
                             32,
                             WGL_DEPTH_BITS_ARB,
                             24,
                             WGL_STENCIL_BITS_ARB,
                             8,
                             WGL_SAMPLE_BUFFERS_ARB,
                             1,
                             WGL_SAMPLES_ARB,
                             8, // TODO: set this as an external option (among other suff).
                             0 };

    int pixelformat;
    wglChoosePixelFormatARB(hdc, pixel_attribis, 0, 1, &pixelformat, &pixelformatcount);
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(hdc, pixelformat, sizeof(pfd), &pfd);
    SetPixelFormat(hdc, pixelformat, &pfd);

    GLint contextattribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,    3, WGL_CONTEXT_MINOR_VERSION_ARB, 2, WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };
    tgl__win32_glcontext = wglCreateContextAttribsARB(hdc, 0, contextattribs);

    tgl_wglMakeCurrent(hdc, tgl__win32_glcontext);

    DEBUG_tgl_init_function_pointers();

    tgl_context_info_get(&tgl__win32_context_info);
    tgl_context_info_printf(&tgl__win32_context_info);

    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(hdc);

    tg_printf("OpenGL Version: %s", glGetString(GL_VERSION));
}

void DEBUG_tgl_shutdown()
{
    tgl_wglDeleteContext(tgl__win32_glcontext);
}

#elif defined(__wasm__)

#include "tg_wasm.h"

// clang-format off
extern void tg__wasm_js_init_gl_context();
TG_WASM_JS(
        function tg__wasm_js_init_gl_context() {
            if (!canvas) { throw("No canvas!"); }

            ctx = canvas.getContext("webgl");

            if (!ctx) { throw("Failed to create context!"); }
        }
)

extern void tg__wasm_js_gl_clear_color(float r, float g, float b, float a);
TG_WASM_JS(
        function tg__wasm_js_gl_clear_color(r,g,b,a) {
            ctx.clearColor(r,g,b,a);
        }
)

extern void tg__wasm_js_gl_clear(GLbitfield mask);
TG_WASM_JS(
        function tg__wasm_js_gl_clear(mask) {
            ctx.clear(mask);
        }
)

extern GLuint tg__wasm_js_glCreateShader(GLenum shaderType); 
TG_WASM_JS(
        function tg__wasm_js_glCreateShader(shaderType) {
            return glShaders.add(ctx.createShader(shaderType));
        }
)

extern GLuint tg__wasm_js_glCreateProgram(void); 
TG_WASM_JS(
        function tg__wasm_js_glCreateProgram() {
            return glPrograms.add(ctx.createProgram());
        }
)


extern void tg__wasm_js_glCompileShader(GLuint shader);
TG_WASM_JS(
        function tg__wasm_js_glCompileShader(shader) {
            ctx.compileShader(glShaders.get(shader));
        }
)

extern void tg__wasm__js_glDeleteShader(GLuint shader);
TG_WASM_JS(
        function tg__wasm__js_glDeleteShader(shader) {
            ctx.deleteShader(glShaders.get(shader));
        }
)

extern GLuint tg__wasm__js_getShaderParameter(GLuint shader, GLenum pname);
TG_WASM_JS(
        function tg__wasm__js_getShaderParameter(shader, pname) {
            return ctx.getShaderParameter(glShaders.get(shader), pname);
        }
)

void tg__wasm__js_glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    *params = tg__wasm__js_getShaderParameter(shader, pname);
}

extern void tg__wasm__js_glShaderSource(GLuint shader, char *src);
TG_WASM_JS(
        function tg__wasm__js_glShaderSource(shader, ptr) {
            let string = decodeStringAt(ptr);
            ctx.shaderSource(glShaders.get(shader), string);
        }
)

void tg__wasm__glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
   //TODO: For now we only accept a single string. Fix this to conform to standard later.
   tg__wasm__js_glShaderSource(shader, (char *) string[0]);
}

extern void tg__wasm__js_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
TG_WASM_JS(
        function tg__wasm__js_glGetShaderInfoLog(shader, bufSize, lenPtr, strPtr) {
            let log = ctx.getShaderInfoLog(glShaders.get(shader));
            if (strPtr) {
            encodeStringAt(log, strPtr, bufSize);
                if (lenPtr) {
                    u32[lenPtr / 4] = log.length;
                }
            }
        }
)

extern void tg__wasm__js_glLinkProgram(GLuint program);
TG_WASM_JS(
        function tg__wasm__js_glLinkProgram(program) {
           ctx.linkProgram(glPrograms.get(program)); 
        }
)

extern GLuint tg__wasm__js_glCrateProgram(void);
TG_WASM_JS(
        function tg__wasm__js_glCrateProgram() {
            return addProgram(ctx.createProgram());
        }
)

extern void tg__wasm__js_glAttachShader(GLuint program, GLuint shader);
TG_WASM_JS(
        function tg__wasm__js_glAttachShader(program, shader) {
            ctx.attachShader(glPrograms.get(program), glShaders.get(shader));
        }
)

extern void tg__wasm__js_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
TG_WASM_JS(
        function tg__wasm__js_glGetProgramiv(program, pname, ptr) {
            let value = ctx.getProgramParameter(glPrograms.get(program), pname);

            u32[ptr/4] = value;
       }
)

extern void tg__wasm__js_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
TG_WASM_JS(
        function tg__wasm__js_glGetProgramInfoLog(program, bufSize, lenPtr, strPtr) {
            let log = ctx.getProgramInfoLog(glPrograms.get(program));
            if (strPtr) {
            encodeStringAt(log, strPtr, bufSize);
                if (lenPtr) {
                    u32[lenPtr / 4] = log.length;
                }
            }
        }
)

extern void tg__wasm__js_glUseProgram(GLuint program);
TG_WASM_JS(
        function tg__wasm__js_glUseProgram(program) {
            ctx.useProgram(glPrograms.get(program));
        }
)


extern void tg__wasm__js_glDrawArrays(GLenum mode, GLint first, GLsizei count);
TG_WASM_JS(
        function tg__wasm__js_glDrawArrays(mode, first, count) {
            ctx.drawArrays(mode, first, count);
        }
)

TG_WASM_JS(
        let glBuffers = new ObjectPool();
)

extern void tg__wasm__js_glGenBuffers(GLsizei n, GLuint *buffers);
TG_WASM_JS(
        function tg__wasm__js_glGenBuffers(n, buffersPtr) {
            for (let i = 0; i < n; i++) {
                u32[buffersPtr/4 + i] = glBuffers.add(ctx.createBuffer());
            }
        }
)

extern void tg__wasm__js_glBindBuffer(GLenum target, GLuint buffer);
TG_WASM_JS(
        function tg__wasm__js_glBindBuffer(target, buffer) {
            ctx.bindBuffer(target, glBuffers.get(buffer));
        }
)

extern void tg__wasm__js_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
TG_WASM_JS(
        function tg__wasm__js_glBufferData(target, size, dataPtr, usage) {
            buffer = memory.buffer.slice(dataPtr, dataPtr + size);
            ctx.bufferData(target, buffer, usage);
        }
)

extern void tg__wasm__js_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
TG_WASM_JS(
        function tg__wasm__js_glBufferSubData(target, offset, size, dataPtr) {
            buffer = memory.buffer.slice(dataPtr, dataPtr + size);
            ctx.bufferSubData(target, offset, buffer);
        }
)

extern void tg__wasm__js_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
TG_WASM_JS(
        function tg__wasm__js_glVertexAttribPointer(index, size, type, normalized, stride, pointer) {
            ctx.vertexAttribPointer(index, size, type, normalized, stride, pointer);
        }
)

extern void tg__wasm__js_glEnableVertexAttribArray(GLuint index);
TG_WASM_JS(
        function tg__wasm__js_glEnableVertexAttribArray(index) {
            ctx.enableVertexAttribArray(index);
        }
)

extern void tg__wasm__js_glDisableVertexAttribArray(GLuint index);
TG_WASM_JS(
        function tg__wasm__js_glDisableVertexAttribArray(index) {
            ctx.disableVertexAttribArray(index);
        }
)

extern void tg__wasm__js_glGetIntegerv(GLenum value, int *data);
TG_WASM_JS(
        function tg__wasm__js_glGetIntegerv(value, data) {
            return ctx.getParameter(value);
        }
)

const GLubyte* tg__wasm__glGetString(GLenum name)
{
    return (const unsigned char *) "glGetString not implemented!";
}

extern void tg__wasm__js_glVertexAttrib1f(GLuint index, float value);
TG_WASM_JS(
        function tg__wasm__js_glVertexAttrib1f(index, value) {
            ctx.vertexAttrib1f(index, value);
        }
)

// clang-format off

void DEBUG_tgl_init_function_pointers()
{
    glClearColor = tg__wasm_js_gl_clear_color;
    glClear = tg__wasm_js_gl_clear;
    glCreateShader = tg__wasm_js_glCreateShader;
    glCompileShader = tg__wasm_js_glCompileShader;
    glDeleteShader = tg__wasm__js_glDeleteShader;
    glGetShaderInfoLog = tg__wasm__js_glGetShaderInfoLog;
    glGetShaderiv = tg__wasm__js_glGetShaderiv;
    glCreateProgram = tg__wasm_js_glCreateProgram;
    glShaderSource = tg__wasm__glShaderSource;
    glLinkProgram = tg__wasm__js_glLinkProgram;
    glAttachShader = tg__wasm__js_glAttachShader;
    glGetProgramiv = tg__wasm__js_glGetProgramiv;
    glGetProgramInfoLog = tg__wasm__js_glGetProgramInfoLog;
    glUseProgram = tg__wasm__js_glUseProgram;
    glDrawArrays = tg__wasm__js_glDrawArrays;
    glGenBuffers = tg__wasm__js_glGenBuffers;
    glBindBuffer = tg__wasm__js_glBindBuffer;
    glBufferData = tg__wasm__js_glBufferData;
    glBufferSubData = tg__wasm__js_glBufferSubData;
    glVertexAttribPointer = tg__wasm__js_glVertexAttribPointer;
    glEnableVertexAttribArray = tg__wasm__js_glEnableVertexAttribArray;
    glDisableVertexAttribArray = tg__wasm__js_glDisableVertexAttribArray;
    glGetIntegerv = tg__wasm__js_glGetIntegerv;
    glGetString = tg__wasm__glGetString;

#ifdef TGL_WEBGL2
    // Get all WebGL2 pointers here

    //TGL_GETPROC(glGenVertexArrays);
    //TGL_GETPROC(glBindVertexArray);
#endif
}

void DEBUG_tgl_init() {
    tg__wasm_js_init_gl_context();

    DEBUG_tgl_init_function_pointers();


    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

#endif

#endif
