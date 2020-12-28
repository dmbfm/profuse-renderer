#ifndef __PLATFORM_H
#define __PLATFORM_H 

#include "common.h"
#include "result.h"
#include "maybe.h"
#include "allocator.h"

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
        int x;
        int y;
        int last_x;
        int last_y;
        int delta_x;
        int delta_y;
        int mouse_wheel_delta;
        PlatformButtonState left_button;
        PlatformCursorStyle cursor_style;
    } mouse;

    struct
    {
        u32 counter;
    } timing;

    void *user_context;

} Platform;

// "User-defined" functions
Platform p_config();
void p_init(Platform *p);
void p_frame(Platform *p);
void p_shutdown(Platform *p);

// Platform API
void platform_print_fmt(const char *fmt, ...);
void platform_print_line(const char *string);

#endif /* __PLATFORM_H */

