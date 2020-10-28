#ifndef _TG_PLATFORM_H
#define _TG_PLATFORM_H

#include "tg_types.h"

#ifdef _WIN32
#include <Windows.h>
#endif

struct tgp_PlatformData;

typedef struct tgp_Platform
{
    struct
    {
        char *title;

        u32 x;
        u32 y;
        u32 width;
        u32 height;

    } window;

    struct tgp_PlatformData *data;

} tgp_Platform;

boolean tgp_init_window(tgp_Platform *p);
boolean tgp_init(tgp_Platform *p);

int tg_main(int argc, char *argv[]);

// =========================================================================
//
//  Web Platform Implementation
//
// =========================================================================

// TODO: dumb shit so clangd works inside __wasm__ blocks for autocompletion
#define __wasm__
#ifdef _WIN32
#undef __wasm__
#endif

#ifdef __wasm__
#include "tg_wasm.h"

// ---------------------
// Imported js functions
// ---------------------

extern u32 tg__wasm_js_get_memory_size(void);

// clang-format off
TG_WASM_JS(
    function tg__wasm_js_get_memory_size() {
        return memory.buffer.byteLength;
    }
)
// clang-format on

// ------------------
// WebAssembly memory
// ------------------
extern u8 __heap_base;

typedef struct WasmMemory
{
    iptr base;
    u32 size;
    u32 current;
} WasmMemory;

static WasmMemory memory;

// export_named(malloc)
export_named(malloc) void *tg__wasm_request_memory(int size)
{
    u64 current_mem_size = tg__wasm_js_get_memory_size();
    memory.base          = (u32)&__heap_base;

    if (memory.current + size > current_mem_size) {
        return 0;
    }

    memory.current += size;

    return (void *)(memory.base - size + memory.current);
}

void tg_wasm_free(void *p) {}

#define tg_malloc        tg__wasm_request_memory
#define tg_realloc(a, s) tg__wasm_request_memory((s))
#define tg_free          tg_wasm_free

struct tgp_PlatformData
{
    u32 dummy;
};

export int main(int argc, char **argv)
{
    tg_main(argc, argv);
}

#endif

// =========================================================================
//
//  Win32 Platform Implementation
//
// =========================================================================

#define TG_WIN32_PRINT_TO_DEBUG_CONSOLE
#define TG_WIN32_PRINT_TO_DEBUG_CONSOLE_AND_STDOUT

#ifdef _WIN32

#include <Windows.h>

#ifdef TG_WIN32_PRINT_TO_DEBUG_CONSOLE
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
#endif // TG_WIN32_PRINT_TO_DEBUG_CONSOLE_AND_STDOUT
#endif // TG_WIN32_PRINT_TO_DEBUG_CONSOLE

struct tgp_PlatformData
{
    HWND window_handle;
};

struct tgp_PlatformData tgp__win32_platform_data;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    tg_main(0, 0);

    return 0;
}

LRESULT CALLBACK tg_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

boolean tgp_init_window(tgp_Platform *p)
{
    if (!p->window.title) {
        p->window.title = "Toilet Games Platform Window";
    }

    if (p->window.width == 0) {
        p->window.width = 800;
    }

    if (p->window.height == 0) {
        p->window.height = 600;
    }

    WNDCLASS wc = { 0 };

    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc   = tg_window_proc;
    wc.hInstance     = GetModuleHandleA(0);
    wc.lpszClassName = "TGPCLASSNAME";

    RegisterClass(&wc);

    HWND wh = CreateWindow(wc.lpszClassName, p->window.title, WS_OVERLAPPEDWINDOW, p->window.x, p->window.y,
                           p->window.width, p->window.height, 0, 0, wc.hInstance, 0);
    if (!wh) {
        return false;
    }

    p->data->window_handle = wh;

    ShowWindow(wh, SW_SHOW);

    return true;
}

boolean tgp_init(tgp_Platform *p)
{
    p->data = &tgp__win32_platform_data;

    if (!tgp_init_window(p))
    {
        return false;
    }

    return true;
}

#endif // _WIN32

#endif // _TG_PLATFORM_H
