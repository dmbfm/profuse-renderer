#include "common.h"

// clang-format off
extern u32 wasm_get_memory_size();
WASM_JS(
function wasm_get_memory_size() {
        return memory.buffer.byteLength;
}
)

// clang-format on
