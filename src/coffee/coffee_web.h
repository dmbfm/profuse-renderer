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

extern void c__wasm_js_print_integer(int x);
WASM_JS(
        function c__wasm_js_print_integer(x) { 
            console.log(x);
        }
)

extern void c__wasm_js_print_f32(f32 x);
WASM_JS(
        function c__wasm_js_print_f32(x) { 
            console.log(x);
        }
)

// clang-format on

#define t__printf(...) \
{\
    char __t_printf_buf##__LINE__[256]; \
    t_snprintf(__t_printf_buf##__LINE__, 256, ##__VA_ARGS__);\
    c__wasm_js_print_line(__t_printf_buf##__LINE__);\
}

#define t__printf_alloc(...) \
{\
    size_t _t_printf_len##__LINE__ = t_snprintf(0, 0, ##__VA_ARGS__);\
    char *_t_printf_b##__LINE__ = (char *) t_malloc( _t_printf_len##__LINE__ + 1000);\
    t_assert(_t_printf_b##__LINE__);\
    t_snprintf(_t_printf_b##__LINE__, _t_printf_len##__LINE__, ##__VA_ARGS__);\
    c__wasm_js_print_line(_t_printf_b##__LINE__);\
    t_free(_t_printf_b##__LINE__);\
}

export_named(malloc) void *coffee_wasm_request_memory(int size);
void coffee_wasm_free(void *p);

#define t_printf        t__printf_alloc
#define t_malloc        coffee_wasm_request_memory
#define t_realloc(a, s) coffee_wasm_request_memory((s))
#define t_free          coffee_wasm_free
#define t_sqrtf         c__wasm_js_sqrtf
#define t_tanf          c__wasm_js_tanf
#define t_sinf          c__wasm_js_sinf
#define t_cosf          c__wasm_js_cosf

#endif
#endif
