#include "mem_common.h"
#include "platform.h"
#include "result.h"
#include "format.h"
#include "math.h"
#include "heap.h"

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

export int main(int argc, char **argv)
{
    platform_print_fmt("Hello!");

    heap_wasm_free_list_allocator.alloc(&heap_wasm_free_list_allocator, 20);
}

