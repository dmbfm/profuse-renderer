#include "common.h"
#include "heap.h"
#include "platform.h"
#include "platform_gl.h"
#include "renderer_gl.h"
#include "result.h"
#include "utils.h"

#ifdef __wasm__
Allocator *a = &heap_wasm_free_list_allocator;
#else
Allocator *a = &heap_allocator;
#endif

char *vshader =
#ifdef __wasm__
    "#version 130\n"
#else
    "#version 330\n"
#endif
    "attribute vec3 pos;"
    "void main() {\n"
    "gl_Position = vec4(pos, 1);\n"
    "}";

char *fshader =
#ifdef __wasm__
    "#version 130\n"
#else
    "#version 330\n"
#endif

    "void main() {\n"
    "gl_FragColor = vec4(1.0f, 0, 0, 1.0f);\n"
    "}";

Platform p_config(void)
{
    Platform p = { 0 };

    return p;
}

void p_init(Platform *p)
{
    UNUSED_VARIABLE(p);

    platform_print_line("p_init");

    Result(uptr) rx = a->alloc(a, sizeof(i32));

    i32 *x = (i32 *)result_unwrap(rx);

    *x = 10;
    platform_print_fmt(a, "x = %d", *x);

    DEBUGLOG("[main]: message");

    a->free(a, (uptr)x);

    rgl_create_program_raw(a, vshader, fshader);
}

void p_frame(Platform *p)
{
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void p_shutdown(Platform *p)
{
    platform_print_line("p_shutdown");
}
