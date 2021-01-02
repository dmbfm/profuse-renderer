#include "common.h"
#include "platform.h"
#include "platform_gl.h"
#include <stdarg.h>
#include <stdio.h>

#include "gen/win32_gl.c"

DECL_FUNC_POINTER(static, PROC, wglGetProcAdressFunc, wgl_get_proc_address, LPCSTR name);
DECL_FUNC_POINTER(static, HGLRC, wglCreateContextFunc, wgl_create_context, HDC Arg1);
DECL_FUNC_POINTER(static, BOOL, wglDeleteContextFunc, wgl_delete_context, HGLRC Arg);
DECL_FUNC_POINTER(static, BOOL, wglMakeCurrentFunc, wgl_make_current, HDC Arg1, HGLRC Arg2);

DECL_FUNC_POINTER(
    static, BOOL, wglChoosePixelFormatARBFunc, wgl_choose_pixel_format_arb, HDC hdc, const int *piAttribIList,
    const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

DECL_FUNC_POINTER(
    static, HGLRC, wglCreateContextAttribsARBFunc, wgl_create_context_attribs_arb, HDC hDC, HGLRC hshareContext,
    const int *attribList);

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

static ErrorCode platform_win32_load_wgl()
{
    HMODULE hgl = LoadLibraryA("opengl32.dll");

    if (hgl == 0)
        return ERR_PLATFORM_WIN32_OPENGL32_DLL;

    wgl_get_proc_address = (wglGetProcAdressFunc *)GetProcAddress(hgl, "wglGetProcAddress");
    wgl_create_context   = (wglCreateContextFunc *)GetProcAddress(hgl, "wglCreateContext");
    wgl_delete_context   = (wglDeleteContextFunc *)GetProcAddress(hgl, "wglDeleteContext");
    wgl_make_current     = (wglMakeCurrentFunc *)GetProcAddress(hgl, "wglMakeCurrent");

    assert(wgl_get_proc_address);
    assert(wgl_create_context);
    assert(wgl_delete_context);
    assert(wgl_make_current);

    if (!wgl_get_proc_address)
        return ERR_PLATFORM_WIN32_OPENGL32_DLL;

    return ERR_OK;
}

static ErrorCode platform_win32_init_wgl_extensions()
{
    WNDCLASS wc = { .style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
                    .lpfnWndProc   = DefWindowProc,
                    .hInstance     = GetModuleHandle(0),
                    .lpszClassName = "PlatformTempWindowClass" };

    RegisterClass(&wc);

    HWND wh = CreateWindow(
        wc.lpszClassName,
        "PlatformTempoWindow",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        wc.hInstance,
        0);

    if (!wh)
        return ERR_ANY;

    HDC dc = GetDC(wh);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
                                  1,
                                  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                                  PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
                                  32,            // Colordepth of the framebuffer.
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  24, // Number of bits for the depthbuffer
                                  8,  // Number of bits for the stencilbuffer
                                  0,  // Number of Aux buffers in the framebuffer.
                                  PFD_MAIN_PLANE,
                                  0,
                                  0,
                                  0,
                                  0 };

    int cpf = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, cpf, &pfd);

    HGLRC glc = wgl_create_context(dc);

    if (!glc)
        return ERR_ANY;

    wgl_make_current(dc, glc);

    wgl_choose_pixel_format_arb = (wglChoosePixelFormatARBFunc *)wgl_get_proc_address("wglChoosePixelFormatARB");
    wgl_create_context_attribs_arb =
        (wglCreateContextAttribsARBFunc *)wgl_get_proc_address("wglCreateContextAttribsARB");

    assert(wgl_choose_pixel_format_arb);
    assert(wgl_create_context_attribs_arb);

    if (!wgl_create_context_attribs_arb || !wgl_choose_pixel_format_arb)
        return ERR_ANY;

    wgl_make_current(dc, 0);
    wgl_delete_context(glc);
    ReleaseDC(wh, dc);
    DestroyWindow(wh);

    return ERR_OK;
}

static void *gl_get_proc_adress(const char *name)
{
    void *p = (void *)wgl_get_proc_address(name);
    if (p == 0 || (p == (void *)0x1) || (p == (void *)0x2) || (p == (void *)0x3) || (p == (void *)-1)) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p              = (void *)GetProcAddress(module, name);
    }

    return p;
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

static void platform_win32_init_window(Platform *p)
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

static ErrorCode platform_win32_init_gl(Platform *p)
{
    // Load wgl function pointers
    error_raise(platform_win32_load_wgl());

    // Load wgl extensions for context creation
    error_raise(platform_win32_init_wgl_extensions());

    // Set the pixel format
    int pixel_attribs[] = { WGL_DRAW_TO_WINDOW_ARB,
                            GL_TRUE,
                            WGL_SUPPORT_OPENGL_ARB,
                            GL_TRUE,
                            WGL_ACCELERATION_ARB,
                            WGL_FULL_ACCELERATION_ARB,
                            WGL_DOUBLE_BUFFER_ARB,
                            GL_TRUE,
                            WGL_PIXEL_TYPE_ARB,
                            WGL_TYPE_RGBA_ARB,
                            WGL_COLOR_BITS_ARB,
                            32,
                            WGL_DEPTH_BITS_ARB,
                            24,
                            WGL_STENCIL_BITS_ARB,
                            8,
                            WGL_SAMPLE_BUFFERS_ARB,
                            1,
                            WGL_SAMPLES_ARB,
                            8, // TODO: set this as an external option (among other suff).
                            0 };

    int pixelformat       = 0;
    uint pixelformatcount = 0;
    wgl_choose_pixel_format_arb(p->win32.device_context, pixel_attribs, 0, 1, &pixelformat, &pixelformatcount);

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    DescribePixelFormat(p->win32.device_context, pixelformat, sizeof(pfd), &pfd);
    SetPixelFormat(p->win32.device_context, pixelformat, &pfd);

    int context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,    3, WGL_CONTEXT_MINOR_VERSION_ARB, 2, WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };

    HGLRC glc = wgl_create_context_attribs_arb(p->win32.device_context, 0, context_attribs);

    assert(glc);

    if (!glc)
        return ERR_ANY;

    wgl_make_current(p->win32.device_context, glc);

    // Load gl function pointers (since we can only load them after the context is created...)
    platform_win32_gl_init_function_pointers();

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(p->win32.device_context);

    return ERR_OK;
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

static void platform_win32_push(Platform *p) {
    SwapBuffers(p->win32.device_context);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNUSED_VARIABLE(hInstance);
    UNUSED_VARIABLE(hPrevInstance);
    UNUSED_VARIABLE(lpCmdLine);
    UNUSED_VARIABLE(nShowCmd);

    platform = p_config();

    platform_win32_init_window(&platform);
    platform_win32_init_gl(&platform);

    p_init(&platform);

    while (!platform.should_quit) {
        platform_win32_pull(&platform);

        p_frame(&platform);

        platform_win32_push(&platform);
    }

    p_shutdown(&platform);

    return 1;
}
