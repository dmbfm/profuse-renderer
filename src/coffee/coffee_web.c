#include "coffee_web.h"
#include "coffee.h"
#include "coffee_web_js.c"
#include "toolbox_types.h"

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

    return true;
}

