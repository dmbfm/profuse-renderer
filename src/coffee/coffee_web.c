#include "coffee_web.h"
#include "coffee.h"
#include "coffee_web_js.c"
#include "toolbox_common.h"
#include "coffee_gl.h"
extern u8 __heap_base;
typedef struct WasmMemory
{
    iptr base;
    u32 size;
    u32 current;
} WasmMemory;

static WasmMemory memory;

void *coffee_wasm_request_memory(int size)
{
    u64 current_mem_size = c__wasm_js_get_memory_size();
    memory.base          = (u32)&__heap_base;

    t_assert(memory.current + size < current_mem_size);

    if (memory.current + size > current_mem_size) {
        return 0;
    }

    memory.current += size;

    return (void *)(memory.base - size + memory.current);
}

void coffee_wasm_free(void *p) {}

export int main(int argc, char **argv)
{
    return coffee_main(argc, argv);
}

export void frame(Coffee *c)
{
    coffee_frame(c);
}

void coffee_run(Coffee *p)
{
    c__wasm_js_start_main_loop(p);
}

void tgp_pull_mouse(Coffee *c)
{
    // TODO: I gues we.... don't need? just direct memory access in the js event listener
}

void tgp_pull(Coffee *c) {}

void tgp_push(Coffee *c) {}
local_function void coffee_web_init_gl_function_pointers()
{
    glClearColor = c__wasm_js_gl_clear_color;
    glClear = c__wasm_js_gl_clear;
    glCreateShader = c__wasm_js_glCreateShader;
    glCompileShader = c__wasm_js_glCompileShader;
    glDeleteShader = c__wasm__js_glDeleteShader;
    glGetShaderInfoLog = c__wasm__js_glGetShaderInfoLog;
    glGetShaderiv = c__wasm__js_glGetShaderiv;
    glCreateProgram = c__wasm_js_glCreateProgram;
    glShaderSource = c__wasm__glShaderSource;
    glLinkProgram = c__wasm__js_glLinkProgram;
    glAttachShader = c__wasm__js_glAttachShader;
    glGetProgramiv = c__wasm__js_glGetProgramiv;
    glGetProgramInfoLog = c__wasm__js_glGetProgramInfoLog;
    glUseProgram = c__wasm__js_glUseProgram;
    glDrawArrays = c__wasm__js_glDrawArrays;
    glGenBuffers = c__wasm__js_glGenBuffers;
    glBindBuffer = c__wasm__js_glBindBuffer;
    glBufferData = c__wasm__js_glBufferData;
    glBufferSubData = c__wasm__js_glBufferSubData;
    glVertexAttribPointer = c__wasm__js_glVertexAttribPointer;
    glEnableVertexAttribArray = c__wasm__js_glEnableVertexAttribArray;
    glDisableVertexAttribArray = c__wasm__js_glDisableVertexAttribArray;
    glGetIntegerv = c__wasm__js_glGetIntegerv;
    glGetString = c__wasm__glGetString;

#ifdef COFFEE_GL_WEBGL2
    // Get all WebGL2 pointers here

    //TGL_GETPROC(glGenVertexArrays);
    //TGL_GETPROC(glBindVertexArray);
#endif
}

local_function 
boolean coffee_web_init_gl_context()
{
    c__wasm_js_init_gl_context();
    coffee_web_init_gl_function_pointers();

    return true;
}

boolean coffee_init(Coffee *c)
{
    if (!c->window.title) {
        c->window.title = "Toilet Games Platform Window";
    }

    if (c->window.width == 0) {
        c->window.width = 800;
    }

    if (c->window.height == 0) {
        c->window.height = 600;
    }

    c__wasm_js_init_canvas(c->window.width, c->window.height);
    c__wasm_js_set_mouse_pointers(&c->mouse.x, &c->mouse.y);

    coffee_web_init_gl_context();

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}


