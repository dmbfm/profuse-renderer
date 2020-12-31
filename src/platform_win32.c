#define WIN32_LEAN_AND_MEAN
#include "common.h"
#include "platform.h"
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

static Platform platform;

void platform_print_line(const char *string)
{
    OutputDebugStringA(string);
    OutputDebugStringA("\n");
}

void platform_print_fmt(Allocator *a, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (!a) {
        char buf[256];
        vsnprintf(buf, 256, fmt, args);
        platform_print_line(buf);
    }

    va_end(args);
}

static LRESULT CALLBACK platform_win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Platform *p    = (Platform *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    LRESULT result = 0;

    switch (uMsg) {
        case WM_DESTROY:
            {
                p->should_quit = true;
            }
            break;
        default:
            {
                result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }
    }

    return result;
}

static void platform_win32_create_window(Platform *p)
{
    // Register window class
    WNDCLASS windowclass = { 0 };

    windowclass.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    windowclass.lpszClassName = "ProfuseWindowClass";
    windowclass.hInstance     = GetModuleHandleA(0);
    windowclass.lpfnWndProc   = platform_win32_window_proc;

    RegisterClass(&windowclass);

    // Create window with handle
    HWND windowhandle = CreateWindowEx(
        0,
        windowclass.lpszClassName,
        "ProfuseWindowName",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        windowclass.hInstance,
        0);

    if (windowhandle == NULL) {
        OutputDebugString("Failed to create window!\n");
        ExitProcess(-1);
    }

    // Save handle for later use
    p->win32.window_handle = windowhandle;

    // Save a pointer for the platform state in the window state
    SetWindowLongPtr(windowhandle, GWLP_USERDATA, (LONG_PTR)p);

    // Show the window
    ShowWindow(windowhandle, SW_SHOW);

    // Get and save the device context
    p->win32.device_context = GetDC(windowhandle);
}

static void platform_win32_pull(Platform *p)
{
    platform.timing.counter++;

    MSG msg;
    while (PeekMessage(&msg, p->win32.window_handle, 0, 0, PM_REMOVE) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

static void platform_win32_push(Platform *p) {}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNUSED_VARIABLE(hInstance);
    UNUSED_VARIABLE(hPrevInstance);
    UNUSED_VARIABLE(lpCmdLine);
    UNUSED_VARIABLE(nShowCmd);

    platform = p_config();

    p_init(&platform);

    platform_win32_create_window(&platform);

    while (!platform.should_quit) {
        platform_win32_pull(&platform);

        p_frame(&platform);

        platform_win32_push(&platform);
    }

    p_shutdown(&platform);

    return 1;
}

