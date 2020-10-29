//#define __TG_RUN_TESTS

#ifndef __wasm__
#include <stdio.h>
#include <stdlib.h>
#endif

#include "tg_wasm.h"
#include "tg_debug.h"
#include "tg_platform.h"
#include "tg.h"
#include "tg_math.h"

#include "tg_gl.h"

void tgp_frame(tgp_Platform *p)
{
    tgp_pull(p);

    float r = (float)p->mouse.x / p->window.width; 
    float g = (float)p->mouse.y / p->window.height; 

    glClearColor(r, g, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    tgp_push(p);
}

tgp_Platform p = { .window = { .width = 1024, .height = 768, .x = 100, .y = 100, .title = "Profuse-renderer" } };
int tg_main(int argc, char *argv[])
{
#ifdef __TG_RUN_TESTS
    //tg_h_test();
    //tg_math_test();
#endif

   // p.user_context = (void *) &state;
    tgp_init(&p);


#ifdef _WIN32
    DEBUG_tgl_init(p.win32.window_handle);
#elif defined(__wasm__)
    DEBUG_tgl_init(); 
#endif

    tgp_start_loop(&p);

    return 0;
}

