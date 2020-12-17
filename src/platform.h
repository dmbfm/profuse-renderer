#ifndef __PLATFORM_H
#define __PLATFORM_H 

#include "common.h"
#include "result.h"
#include "maybe.h"

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

    void *user_context;

} Platform;

ErrorCode platform_init(Platform *p);
ErrorCode platform_deinit(Platform *p);
void platform_run(Platform *p);

Result(uptr) platform_memory_alloc(usize amount);
Result(uptr) platform_memory_realloc(usize amount);
void platform_memory_free(uptr pointer);

void platform_print_fmt(const char *fmt, ...);
void platform_print_line(const char *string);
void platform_print_u32(u32);
void platform_print_f32(f32);

#endif /* __PLATFORM_H */
