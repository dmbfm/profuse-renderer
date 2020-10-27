#ifndef _TG_PLATFORM_H
#define _TG_PLATFORM_H

#include "tg_types.h"

typedef struct tgp_Platform
{

} tgp_Platform;

// =========================================================================
//
//  Web Platform Implementation
//
// =========================================================================

#ifdef __wasm__

// ---------------------
// Imported js functions
// ---------------------

extern void tg__wasm_js_print_line(char *s);



// ------------------
// WebAssembly memory
// ------------------
extern u8 __heap_base;
extern u32 tg_wasm_js_get_memory_size(void);

typedef struct WasmMemory
{
    iptr base;
    u32 size;
    u32 current;
} WasmMemory;

static WasmMemory memory;

export_named(malloc) 
void *tg__wasm_request_memory(int size)
{
    u64 current_mem_size = tg_wasm_js_get_memory_size();
    memory.base = (u32) &__heap_base;

    if (memory.current + size > current_mem_size)
    {
        return 0;
    }

    memory.current += size;

    return (void *) (memory.base - size + memory.current);
}

void tg_wasm_free(void *p) {}

#define tg_malloc tg__wasm_request_memory
#define tg_realloc(a,s) tg__wasm_request_memory((s))
#define tg_free tg_wasm_free

// -------------------------
// WebAssembly print/console
// -------------------------
#define tgp__wasm_printf(s, ...)                           \
    {                                               \
        char buffer[256];                           \
        tg_snprintf(buffer, 256, s, ##__VA_ARGS__); \
        tg_wasm_js_print_line((buffer));            \
    }
#endif

// Define stuff that will be consumed by tg.h and others. 
#define tg_printf tgp_wasm_printf

#endif

// =========================================================================
//
//  Win32 Platform Implementation
//
// =========================================================================

#define TG_WIN32_PRINT_TO_DEBUG_CONSOLE
#define TG_WIN32_PRINT_TO_DEBUG_CONSOLE_AND_STDOUT


#if defined(_WIN32) && defined(TG_WIN32_PRINT_TO_DEBUG_CONSOLE)
#define __tg_print_256(fmt, ...)                         \
    {                                                    \
        char buffer[256];                                \
        snprintf(buffer, (size_t)256, fmt, __VA_ARGS__); \
        OutputDebugString(buffer);                       \
    }

#define __tg_print_alloc(fmt, ...)                       \
    {                                                    \
        int size     = snprintf(0, 0, fmt, __VA_ARGS__); \
        char *buffer = (char *)malloc(size + 1);         \
        snprintf(buffer, size + 1, fmt, __VA_ARGS__);    \
        OutputDebugString(buffer);                       \
        free(buffer);                                    \
    }
#ifdef TG_WIN32_PRINT_TO_DEBUG_CONSOLE_AND_STDOUT
#define tg_printf(fmt, ...)     \
    printf(fmt, ##__VA_ARGS__); \
    __tg_print_256(fmt, ##__VA_ARGS__);
#else
#define tg_printf __tg_print_256
#endif


#endif
