//#define __TG_RUN_TESTS

#ifndef __wasm__
#include <stdio.h>
#include <stdlib.h>
#endif

#include "tg_debug.h"
#include "tg_wasm.h"
#include "tg_platform.h"
#include "tg.h"
#include "tg_math.h"

#include "tg_gl.h"

// TODO: Since in WebAssembly we need to have async frame/main loop anyways, and tg_main will actually reutrn before
//       the first frame even starts, it is probably better to do the platform layer handmade-style: tg_init, tg_frame
//       and tg_shutdown.

void tgp_frame(tgp_Platform *p)
{
    tgp_pull(p);

    float r = (float)p->mouse.x / p->window.width;
    float g = (float)p->mouse.y / p->window.height;

    glClearColor(r, g, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    tgp_push(p);
}

char *vshader = "#version 130\n" 
                "attribute vec3 pos;"
                "void main() {\n"
                "gl_Position = vec4(pos, 1);\n"
                "}";

char *fshader = "#version 130\n"
                "void main() {\n"
                "gl_FragColor = vec4(1.0f, 0, 0, 1.0f);\n"
                "}";

const float triangle_vertices[] = {
    -0.5, -0.5, 0,
    0, 0.5, 0,
    0.5, -0.5, 0
};

tgp_Platform p = { .window = { .width = 1024, .height = 768, .x = 100, .y = 100, .title = "Profuse-renderer" } };
int tg_main(int argc, char *argv[])
{
#ifdef __TG_RUN_TESTS
    // tg_h_test();
    // tg_math_test();
#endif

    // p.user_context = (void *) &state;
    tgp_init(&p);

#ifdef _WIN32
    DEBUG_tgl_init(p.win32.window_handle);
#elif defined(__wasm__)
    DEBUG_tgl_init();
#endif

    GLuint program = DEBUG_tgl_create_program(vshader, fshader);
    glUseProgram(program);

    u32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    u32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);

    tgp_start_loop(&p);

    return 0;
}
