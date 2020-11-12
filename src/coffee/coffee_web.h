#ifndef COFFEE_WEB_H
#define COFFEE_WEB_H
#ifdef __wasm__

#include "toolbox_common.h"

#define WASM_JS(...)

#define export __attribute__((used,visibility("default")))
#define export_named(name) __attribute__((used, visibility("default"), export_name(#name)))

export void *memset(void *ptr, int value, size_t num)
{
    unsigned char *p = (unsigned char *) ptr;
    while(num) {
        *p++ = (unsigned char) value;
        num--;
    }

    return ptr;
}

// clang-format off
extern void c__wasm_js_print_line(char *s);
WASM_JS(
function c__wasm_js_print_line(p) { 
  console.log(decodeStringAt(p)); 
}
)
// clang-format on

#define t_printf(...) \
{\
    char __t_printf_buf##__LINE__[256]; \
    t_snprintf(__t_printf_buf##__LINE__, 256, ##__VA_ARGS__);\
    c__wasm_js_print_line(__t_printf_buf##__LINE__);\
}

export_named(malloc) void *coffee_wasm_request_memory(int size);
void coffee_wasm_free(void *p);

#define t_malloc        coffee_wasm_request_memory
#define t_realloc(a, s) coffee_wasm_request_memory((s))
#define t_free          coffee_wasm_free

#endif
#endif
