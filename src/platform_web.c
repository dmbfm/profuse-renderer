#include "format.h"
#include "heap.h"
#include "math.h"
#include "mem_common.h"
#include "platform.h"
#include "result.h"

#include "platform_web_js_imports.c"

typedef struct PlatformWeb
{
    int x;
} PlatformWeb;

static PlatformWeb platform_web;

void platform_print_fmt(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buf[512];
    formatv(buf, 512, fmt, arg);
    wasm_print_line(buf);
}

export int main()
{
    assert(0);
    platform_print_fmt("Hello!");


    Allocator *a = &heap_wasm_free_list_allocator;

    heap_wasm_free_list_allocator.alloc(&heap_wasm_free_list_allocator, 20);
    heap_wasm_free_list_print_blocks(wasm_print_line);

    heap_wasm_free_list_allocator.alloc(&heap_wasm_free_list_allocator, 1024);
    heap_wasm_free_list_print_blocks(wasm_print_line);

    int *x =
        (int *)result_unwrap(heap_wasm_free_list_allocator.alloc(&heap_wasm_free_list_allocator, 100 * sizeof(int)));

    forn(i, 100) {
        x[i] = i;
    }

    forn(i, 100) {
        platform_print_fmt("%i\n", x[i]);
    }

    heap_wasm_free_list_print_blocks(wasm_print_line);

    a->free(a, (uptr)x);

    heap_wasm_free_list_print_blocks(wasm_print_line);
}
