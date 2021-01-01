#include "platform.h"
#include "common.h"
#include "heap.h"
#include "result.h"

#ifdef __wasm__
    Allocator *a = &heap_wasm_free_list_allocator;
#else
    Allocator *a = &heap_allocator;
#endif

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

    i32 *x = (i32 *) result_unwrap(rx);

    *x = 10;
    platform_print_fmt(a, "x = %d", *x);


    a->free(a, (uptr)x);
}

void p_frame(Platform *p)
{
    //platform_print_line("p_frame");

    //if (p->timing.counter >= 10) p->should_quit = true;
}

void p_shutdown(Platform *p)
{
    platform_print_line("p_shutdown");
}
