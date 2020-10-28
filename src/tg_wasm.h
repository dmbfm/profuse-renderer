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

#define tg_printf(...) \
{\
    char buf[256]; \
    tg_snprintf(buf, 256, ##__VA_ARGS__);\
    tg__wasm_js_print_line(buf);\
}
#endif

#ifndef __wasm__
#define export
#define export_named(name)
#endif


#endif
