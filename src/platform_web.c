#include "platform.h"

#include "platform_web_js_exports.c"

extern u8 __heap_base;

typedef struct WasmMemory
{
    uptr base;
    usize current_size;
    usize num_pages;
    usize bytes_per_page;
} WasmMemory;

typedef struct PlatformWeb
{
    WasmMemory memory;
} PlatformWeb;

static PlatformWeb platform_web;

ErrorCode platform_web_init_memory(WasmMemory *memory) {

    memory->base = (uptr) &__heap_base;
    memory->current_size = wasm_get_memory_size();
    // Wasm pages are 64kb in size ;)
    memory->bytes_per_page = KB(64);
    memory->num_pages = memory->current_size / memory->bytes_per_page; 

    if (memory->base == 0 || memory->current_size == 0) {
        return ERR_PLATFORM_WEB_MEM_INIT;
    }

    return ERR_OK;
}


Result(uptr) platform_alloc(usize amount) {

}
