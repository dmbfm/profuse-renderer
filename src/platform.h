#ifndef __PLATFORM_H
#define __PLATFORM_H 

#include "common.h"
#include "result.h"
#include "maybe.h"
#include "allocator.h"

#if defined(_WIN32)
#include "platform_win32.h"
#elif defined(__wasm__)
#include "platform_web.h"
#else
#error "Unsupported platform!"
#endif /* defined(_WIN32) */

typedef enum
{
    PLATFORM_CURSOR_STYLE_NORMAL,
    PLATFORM_CURSOR_STYLE_NONE
} PlatformCursorStyle;

typedef struct
{
    boolean is_down;
    boolean was_down;
} PlatformButtonState;

typedef struct Platform
{
    boolean should_quit;

    struct
    {
        Maybe(char) *title;
        Maybe(u32) x;
        Maybe(u32) y;
        Maybe(u32) width;
        Maybe(u32) heigh;

        boolean resized;
    } window;

    struct
    {
        i32 x;
        i32 y;
        i32 last_x;
        i32 last_y;
        i32 delta_x;
        i32 delta_y;
        i32 mouse_wheel_delta;
        PlatformButtonState left_button;
        PlatformCursorStyle cursor_style;
    } mouse;

    struct
    {
        u32 counter;
    } timing;

    void *user_context;

#if defined(_WIN32)
    PlatformWin32 win32;
#endif

} Platform;

// "User-defined" functions
Platform p_config(void);
void p_init(Platform *p);
void p_frame(Platform *p);
void p_shutdown(Platform *p);

// Platform API
void platform_print_fmt(Allocator *a, const char *fmt, ...);
void platform_print_line(const char *string);

#endif /* __PLATFORM_H */

