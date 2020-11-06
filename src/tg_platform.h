#ifndef _TG_PLATFORM_H
#define _TG_PLATFORM_H

#include "tg_types.h"

#ifdef _WIN32
#include <Windows.h>
#endif

typedef struct tgp_Win32
{
#ifdef _WIN32
    HWND window_handle;
    HDC device_context;
#else
    char _;
#endif
} tgp_Win32;

typedef struct tgp_Wasm
{
#ifdef __wasm__
    char *parent_node_id;
#else
    char _;
#endif
} tgp_Wasm;

typedef struct tgp_Platform
{
    boolean should_quit;

    struct
    {
        char *title;

        u32 x;
        u32 y;
        u32 width;
        u32 height;

        boolean resized;

    } window;

    struct
    {
        int x;
        int y;
    } mouse;

    tgp_Win32 win32;
    tgp_Wasm wasm;

    void *user_context;
} tgp_Platform;

boolean tgp_init(tgp_Platform *p);

void tgp_pull(tgp_Platform *p);
void tgp_push(tgp_Platform *p);

int tg_main(int argc, char *argv[]);
void tgp_frame(tgp_Platform *p);

// =========================================================================
//
//  Web Platform Implementation
//
// =========================================================================

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

// clang-format off
extern void tg__wasm_js_init_canvas(int width, int height);
TG_WASM_JS(
        function tg__wasm_js_init_canvas(width, height) {
            canvas = document.createElement("canvas");
            canvas.width = width;
            canvas.height = height;
            document.body.appendChild(canvas);
            initCanvasEventListeners();
        }
)

extern void tg__wasm_js_start_main_loop(tgp_Platform *p);
TG_WASM_JS(
        function tg__wasm_js_start_main_loop(p) {
        let f = () => {
            instance.exports.frame(p);
            requestAnimationFrame(f);
        }
        f();
}
)

extern void tg__wasm_js_set_mouse_pointers(i32 *x, i32 *y);
TG_WASM_JS(
        function tg__wasm_js_set_mouse_pointers(x,y) {
            mouseXptr = x;
            mouseYptr = y;
        }
)

extern void tg__wasm_js_now(double *out);
TG_WASM_JS(
        function tg__wasm_js_now(ptr) {
            f64[ptr/8] = performance.now();
        }
)
// clang-format on

export int main(int argc, char *argv[])
{
    tg_main(argc, argv);
}

export void frame(tgp_Platform *p)
{
    tgp_frame(p);
}

void tgp_start_loop(tgp_Platform *p)
{
    tg__wasm_js_start_main_loop(p);
}

void tgp_pull_mouse(tgp_Platform *p)
{
    // TODO: I gues we.... don't need? just direct memory access in the js event listener
}

void tgp_pull(tgp_Platform *p) {}

void tgp_push(tgp_Platform *p) {}

boolean tgp_init(tgp_Platform *p)
{
    tg__wasm_js_init_canvas(p->window.width, p->window.height);
    tg__wasm_js_set_mouse_pointers(&p->mouse.x, &p->mouse.y);

    return true;
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

void tgp_start_loop(tgp_Platform *p)
{
    while (!p->should_quit) {
        tgp_frame(p);
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    tg_main(0, 0);

    return 0;
}

LRESULT CALLBACK tg_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    tgp_Platform *p = (tgp_Platform *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    LRESULT result = 0;

    switch (msg) {

    case WM_DESTROY: {
        p->should_quit = true;
    } break;

    case WM_SIZE: {
        p->window.width   = LOWORD(lParam);
        p->window.height  = HIWORD(lParam);
        p->window.resized = true;
    } break;

    case WM_MOUSEMOVE: {
        p->mouse.x = LOWORD(lParam);
        p->mouse.y = HIWORD(lParam);
    } break;

    default: {
        result = DefWindowProc(hwnd, msg, wParam, lParam);
    } break;
    }

    return result;
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

    wc.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = tg_window_proc;
    wc.hInstance     = GetModuleHandleA(0);
    wc.lpszClassName = "TGPCLASSNAME";

    RegisterClass(&wc);
    RECT rect = { .top    = p->window.y,
                  .left   = p->window.x,
                  .bottom = p->window.y + p->window.height,
                  .right  = p->window.x + p->window.width };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);

    HWND wh = CreateWindow(wc.lpszClassName, p->window.title, WS_OVERLAPPEDWINDOW, rect.left, rect.top,
                           rect.right - rect.left, rect.bottom - rect.top, 0, 0, wc.hInstance, 0);
    if (!wh) {
        return false;
    }

    p->win32.window_handle = wh;

    SetWindowLongPtr(wh, GWLP_USERDATA, (LONG_PTR)p);

    ShowWindow(wh, SW_SHOW);

    p->win32.device_context = GetDC(wh);

    return true;
}

boolean tgp_init(tgp_Platform *p)
{
    if (!tgp_init_window(p)) {
        return false;
    }

    return true;
}

void tgp_pull(tgp_Platform *p)
{
    p->window.resized = false;
    MSG msg;
    while (PeekMessage(&msg, p->win32.window_handle, 0, 0, PM_REMOVE) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void tgp_push(tgp_Platform *p)
{
    SwapBuffers(p->win32.device_context);
}

#endif // _WIN32

#endif // _TG_PLATFORM_H
