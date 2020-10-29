#if !defined(_TG_GL_H)
#define _TG_GL_H

#if defined(_WIN32) && !defined(_WIN64)
#define TGLAPI __stdcall
#else
#define TGLAPI
#endif

#define ARGS(X) X

#define TGLFUNC(ret,name,...) \
    typedef ret TGLAPI tgl_##name(__VA_ARGS__);\
    tgl_##name *name;

#include <stdint.h>
typedef float GLfloat;
typedef unsigned int GLbitfield;

TGLFUNC(void, glClearColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
TGLFUNC(void, glClear, GLbitfield mask);

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_NONE 0
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408


#ifdef _WIN32
#include <Windows.h>
#include <wingdi.h>

void *tgl__get_proc_adress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

#define TGL_GETPROC(name) name = (tgl_##name *) tgl__get_proc_adress(#name)
void DEBUG_tgl_init_function_pointers()
{
    TGL_GETPROC(glClear);
    TGL_GETPROC(glClearColor);
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
    
    //tg_printf("OpenGL Version: %s", glGetString(GL_VERSION));
}

#elif defined(__wasm__)

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

void DEBUG_tgl_init_function_pointers()
{
    glClearColor = tg__wasm_js_gl_clear_color;
    glClear = tg__wasm_js_gl_clear;
}
void DEBUG_tgl_init() {
    tg__wasm_js_init_gl_context();

    DEBUG_tgl_init_function_pointers();


    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

#endif

#endif
