#include "coffee_web.h"
#include "coffee.h"
#include "coffee_web_js.c"
#include "toolbox_common.h"
#include "toolbox_memory.h"
#include "coffee_gl.h"

typedef struct CoffeeWasm
{
   i32 mouse_x_raw;
   i32 mouse_y_raw;
   i32 mouse_wheel_delta_raw;
} CoffeeWasm;

static CoffeeWasm coffee_wasm;

extern u8 __heap_base;
typedef struct WasmMemory
{
    iptr base;
    u32 size;
    u32 current;
    u32 last_alloc_size;
} WasmMemory;

ToolboxMemoryArena c_wasm_memory_arena;

void *coffee_wasm_request_memory(size_t size)
{
    return toolbox_memory_arena_alloc(&c_wasm_memory_arena, size);
}

void coffee_wasm_free(void *p) {}

export int main(int argc, char **argv)
{
    toolbox_memory_arena_init(&c_wasm_memory_arena, &__heap_base, c__wasm_js_get_memory_size());
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

void coffee_wasm_pull_mouse(Coffee *c)
{
    c->mouse.last_x = c->mouse.x;
    c->mouse.last_y = c->mouse.y;
    c->mouse.x = coffee_wasm.mouse_x_raw;
    c->mouse.y = coffee_wasm.mouse_y_raw;
    c->mouse.delta_x = c->mouse.x - c->mouse.last_x;
    c->mouse.delta_y = c->mouse.y - c->mouse.last_y;

    c->mouse.mouse_wheel_delta = coffee_wasm.mouse_wheel_delta_raw;
    coffee_wasm.mouse_wheel_delta_raw = 0;
}

void coffee_pull(Coffee *c) {
    coffee_wasm_pull_mouse(c);
}

void coffee_push(Coffee *c) {}

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
    glUniformMatrix4fv = c__wasm_js_glUniformMatrix4fv;
    glGetUniformLocation = c__wasm_js_glGetUniformLocation;
    glEnable = c__wasm_js_glEnable;
    glUniform3fv = c__wasm_js_glUniform3fv;
    glActiveTexture = c__wasm_js_glActiveTexture;
    glGenTextures = c__wasm_js_glGenTextures;
    glBindTexture = c__wasm_js_glBindTexture;
    glTexImage2D = c__wasm_js_glTexImage2D;
    glTexParameteri = c__wasm_js_glTexParameteri;
    glUniform1i = c__wasm_js_glUniform1i;
    glPixelStorei = c__wasm_js_glPixelStorei;
    glBlendFunc = c__wasm_js_glBlendFunc;
    glGenerateMipmap = c__wasm_js_glGenerateMipmap;

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
    //c__wasm_js_set_mouse_pointers(&c->mouse.x, &c->mouse.y);
    c__wasm_js_set_mouse_pointers(&coffee_wasm.mouse_x_raw, &coffee_wasm.mouse_y_raw);
    c__wasm_js_set_mouse_down_pointer(&c->mouse.left_button.is_down);
    c__wasm_js_set_mouse_wheel_pointer(&coffee_wasm.mouse_wheel_delta_raw);
    c__wasm_js_canvas_set_curstor_style(c->mouse.cursor_style);

    coffee_web_init_gl_context();

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}


