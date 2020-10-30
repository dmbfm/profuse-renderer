#if !defined(_TG_GL_H)
#define _TG_GL_H

#if defined(_WIN32) && !defined(_WIN64)
#define TGLAPI __stdcall
#else
#define TGLAPI
#endif

#define ARGS(X) X

#define TGLFUNC(ret, name, ...)                 \
    typedef ret TGLAPI tgl_##name(__VA_ARGS__); \
    tgl_##name *name;

#include <stdint.h>

typedef float GLfloat;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLsizei;
typedef int GLint;
typedef char GLchar;

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
TGLFUNC(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
TGLFUNC(void, glBindVertexArray, GLuint vertexArray);
TGLFUNC(void, glVertexAttrib1f, GLuint index, GLfloat v0);

#define GL_DEPTH_BUFFER_BIT    0x00000100
#define GL_STENCIL_BUFFER_BIT  0x00000400
#define GL_COLOR_BUFFER_BIT    0x00004000
#define GL_FALSE               0
#define GL_TRUE                1
#define GL_POINTS              0x0000
#define GL_LINES               0x0001
#define GL_LINE_LOOP           0x0002
#define GL_LINE_STRIP          0x0003
#define GL_TRIANGLES           0x0004
#define GL_TRIANGLE_STRIP      0x0005
#define GL_TRIANGLE_FAN        0x0006
#define GL_QUADS               0x0007
#define GL_NEVER               0x0200
#define GL_LESS                0x0201
#define GL_EQUAL               0x0202
#define GL_LEQUAL              0x0203
#define GL_GREATER             0x0204
#define GL_NOTEQUAL            0x0205
#define GL_GEQUAL              0x0206
#define GL_ALWAYS              0x0207
#define GL_ZERO                0
#define GL_ONE                 1
#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308
#define GL_NONE                0
#define GL_FRONT_LEFT          0x0400
#define GL_FRONT_RIGHT         0x0401
#define GL_BACK_LEFT           0x0402
#define GL_BACK_RIGHT          0x0403
#define GL_FRONT               0x0404
#define GL_BACK                0x0405
#define GL_LEFT                0x0406
#define GL_RIGHT               0x0407
#define GL_FRONT_AND_BACK      0x0408
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

// TODO: stick this memset somwhere for wasm compilations.
// When you do stuff like char buff[256] = { 0 }; clang needs a memset to be defined.
//


GLuint DEBUG_tgl_create_program(char *vshader, char *fshader)
{
    GLuint vertexshader, fragshader, program;

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexshader, 1, (const char **)&vshader, 0);
    glCompileShader(vertexshader);

    GLint cs = 0;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &cs);

    if (cs == GL_FALSE)
    {
        char buf[256];
        glGetShaderInfoLog(vertexshader, 256, 0, buf);
        tg_printf("VSHADER ERROR: %s\n", buf);

        glDeleteShader(vertexshader);
        return 0;
    }

    glShaderSource(fragshader, 1, (const char **)&fshader, 0);
    glCompileShader(fragshader);

    cs = 0;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &cs);

    if (cs == GL_FALSE)
    {
        char buf[256];
        glGetShaderInfoLog(vertexshader, 256, 0, buf);
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
        //glDeleteProgram(program);
    }

    return program;
}

#ifdef _WIN32
#include <Windows.h>
#include <wingdi.h>

void *tgl__get_proc_adress(const char *name)
{
    void *p = (void *)wglGetProcAddress(name);
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
    TGL_GETPROC(glGenVertexArrays);
    TGL_GETPROC(glBindVertexArray);
    TGL_GETPROC(glVertexAttrib1f);

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
    tg_assert(glGenVertexArrays);
    tg_assert(glBindVertexArray);
    tg_assert(glVertexAttrib1f);
}

void DEBUG_tgl_init(HWND window_handle)
{

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

    HDC hdc = GetDC(window_handle);
    int why = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, why, &pfd);
    HGLRC glcontext = wglCreateContext(hdc);

    wglMakeCurrent(hdc, glcontext);

    DEBUG_tgl_init_function_pointers();

    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(hdc);

    // tg_printf("OpenGL Version: %s", glGetString(GL_VERSION));
}

#elif defined(__wasm__)

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
            return addShader(ctx.createShader(shaderType));
        }
)

extern GLuint tg__wasm_js_glCreateProgram(void); 
TG_WASM_JS(
        function tg__wasm_js_glCreateProgram() {
            return addProgram(ctx.createProgram());
        }
)


extern void tg__wasm_js_glCompileShader(GLuint shader);
TG_WASM_JS(
        function tg__wasm_js_glCompileShader(shader) {
            ctx.compileShader(getShader(shader));
        }
)

extern void tg__wasm__js_glDeleteShader(GLuint shader);
TG_WASM_JS(
        function tg__wasm__js_glDeleteShader(shader) {
            ctx.deleteShader(getShader(shader));
        }
)

extern GLuint tg__wasm__js_getShaderParameter(GLuint shader, GLenum pname);
TG_WASM_JS(
        function tg__wasm__js_getShaderParameter(shader, pname) {
            return ctx.getShaderParameter(getShader(shader), pname);
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
            ctx.shaderSource(getShader(shader), string);
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
            let log = ctx.getShaderInfoLog(getShader(shader));
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
           ctx.linkProgram(getProgram(program)); 
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
            ctx.attachShader(getProgram(program), getShader(shader));
        }
)

extern void tg__wasm__js_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
TG_WASM_JS(
        function tg__wasm__js_glGetProgramiv(program, pname, ptr) {
            let value = ctx.getProgramParameter(getProgram(program), pname);

            u32[ptr/4] = value;
       }
)

extern void tg__wasm__js_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
TG_WASM_JS(
        function tg__wasm__js_glGetProgramInfoLog(program, bufSize, lenPtr, strPtr) {
            let log = ctx.getProgramInfoLog(getProgram(program));
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
            ctx.useProgram(getProgram(program));
        }
)


extern void tg__wasm__js_glDrawArrays(GLenum mode, GLint first, GLsizei count);
TG_WASM_JS(
        function tg__wasm__js_glDrawArrays(mode, first, count) {
            ctx.drawArrays(mode, first, count);
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
}
void DEBUG_tgl_init() {
    tg__wasm_js_init_gl_context();

    DEBUG_tgl_init_function_pointers();


    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

#endif

#endif
