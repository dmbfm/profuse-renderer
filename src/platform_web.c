#include "format.h"
#include "heap.h"
#include "math.h"
#include "mem_common.h"
#include "platform.h"
#include "result.h"

#include "platform_web_js_imports.c"

static Platform platform;

void platform_print_fmt(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buf[512];
    formatv(buf, 512, fmt, arg);
    wasm_print_line(buf);
}

void platform_print_line(const char *string) 
{
    wasm_print_line(string);
}

export int main()
{
    platform_print_line("Hello!");

    platform = p_config();

    p_init(&platform);
}

export int frame(f64 timestamp)
{
    platform.timing.counter++;
    p_frame(&platform);

    return (platform.should_quit == false);
}

export void shutdown()
{
    p_shutdown(&platform);
}
