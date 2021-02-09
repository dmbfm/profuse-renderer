#include "common.h"

extern void wasm_print_line(const char *);
extern void wasm_print_i32(i32 value);
extern void wasm_init_canvas(i32 width, i32 height);
extern void wasm_init_gl_context(void);
extern void wasm_mouse_position(i32 *x, i32 *y);
