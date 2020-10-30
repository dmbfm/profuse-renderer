#if !defined(__TG_WASM_H)
#define __TG_WASM_H


#define TG_WASM_JS(...)

#ifdef __wasm__

#include "tg_types.h"
#include "tg_snprintf.h"

#define export __attribute__((used,visibility("default")))
#define export_named(name) __attribute__((used, visibility("default"), export_name(#name)))
// clang-format off

extern void tg__wasm_js_print_line(char *s);
TG_WASM_JS(
function tg__wasm_js_print_line(p) { 
  console.log(decodeStringAt(p)); 
}
)

// clang-format on

export void *memset(void *ptr, int value, size_t num)
{
    unsigned char *p = (unsigned char *) ptr;
    while(num) {
        *p++ = (unsigned char) value;
        num--;
    }

    return ptr;
}

#define tg_printf(...) \
{\
    char __tg_printf_buf##__LINE__[256]; \
    tg_snprintf(__tg_printf_buf##__LINE__, 256, ##__VA_ARGS__);\
    tg__wasm_js_print_line(__tg_printf_buf##__LINE__);\
}
#endif

#ifndef __wasm__
#define export
#define export_named(name)
#endif


#endif
