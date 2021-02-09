#include "format.h"
#include "heap.h"
#include "math.h"
#include "mem_common.h"
#include "platform.h"
#include "platform_web_js_imports.c"
#include "result.h"

static Platform platform;

void platform_print_fmt(Allocator *a, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    if (!a) {
        char buf[512];
        formatv(buf, 512, fmt, arg);
        wasm_print_line(buf);
    } else {
        usize len          = formatv(0, 0, fmt, arg);
        Result(uptr) rbuff = a->alloc(a, len + 1);
        char *buff         = (char *)result_unwrap(rbuff);
        formatv(buff, len, fmt, arg);
        wasm_print_line(buff);
    }
    va_end(arg);
}

void platform_print_line(const char *string) {
    wasm_print_line(string);
}

void platform_webgl_init_webgl_canvas(Platform *p) {
    wasm_init_canvas(p->window.width.value, p->window.height.value);
    wasm_init_gl_context();
}

export int main() {
    platform_print_line("Hello!");

    platform = p_config();

    platform_init_defaults(&platform);

    platform_webgl_init_webgl_canvas(&platform);

    p_init(&platform);
}

static void platform_web_pull_time(Platform *p) {
    p->timing.frame_count++;
}

static void platform_web_pull_mouse(Platform *p) {
    i32 x, y;
    wasm_mouse_position(&x, &y);
    p->mouse.x = x;
    p->mouse.y = y;
}

static void platform_web_pull(Platform *p) {
    platform_web_pull_time(p);
    platform_web_pull_mouse(p);
}

export int frame(f64 timestamp) {
    //platform.timing.frame_count++;
    platform_web_pull(&platform);

    p_frame(&platform);

    return (platform.should_quit == false);
}

export void shutdown() {
    p_shutdown(&platform);
}
