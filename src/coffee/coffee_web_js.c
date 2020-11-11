#include "coffee.h"
#include "coffee_web.h"

// clang-format off

extern u32 c__wasm_js_get_memory_size(void);
WASM_JS(
    function c__wasm_js_get_memory_size() {
        return memory.buffer.byteLength;
    }
)


extern void c__wasm_js_init_canvas(int width, int height);
WASM_JS(
        function c__wasm_js_init_canvas(width, height) {
            canvas = document.createElement("canvas");
            canvas.width = width;
            canvas.height = height;
            document.body.appendChild(canvas);
            initCanvasEventListeners();
        }
)

extern void c__wasm_js_start_main_loop(Coffee *p);
WASM_JS(
        function c__wasm_js_start_main_loop(p) {
        let f = () => {
            instance.exports.frame(p);
            requestAnimationFrame(f);
        }
        f();
}
)

// NOTE: mouse{X,Y}ptr are defined in 'preface.js', and are updated there via event listeners.
// We just need to set them here at startup.
extern void c__wasm_js_set_mouse_pointers(i32 *x, i32 *y);
WASM_JS(
        function c__wasm_js_set_mouse_pointers(x,y) {
            mouseXptr = x;
            mouseYptr = y;
        }
)

extern void c__wasm_js_now(double *out);
WASM_JS(
        function c__wasm_js_now(ptr) {
            f64[ptr/8] = performance.now();
        }
)
// clang-format on
