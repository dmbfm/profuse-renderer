#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "allocator.h"
#include "common.h"
#include "maybe.h"
#include "result.h"

#if defined(_WIN32)
    #include "platform_win32.h"
#elif defined(__wasm__)
    #include "platform_web.h"
#else
    #error "Unsupported platform!"
#endif /* defined(_WIN32) */

typedef enum {
    PLATFORM_CURSOR_STYLE_NORMAL,
    PLATFORM_CURSOR_STYLE_HAND,
} PlatformCursorStyle;

maybe_make_type(PlatformCursorStyle);

typedef struct {
    boolean is_down;
    boolean is_up;
    boolean was_down;
    boolean was_up;
    boolean just_down;
    boolean just_up;
} PlatformButtonState;

typedef struct Platform {
    boolean should_quit;

    struct {
        Maybe(charptr) title;
        Maybe(u32) x;
        Maybe(u32) y;
        Maybe(u32) width;
        Maybe(u32) height;
        Maybe(PlatformCursorStyle) cursor_style;
        boolean was_resized;

        struct {
            PlatformCursorStyle cursor_style;
        } cached;
    } window;

    struct {
        i32                 x;
        i32                 y;
        i32                 last_x;
        i32                 last_y;
        i32                 delta_x;
        i32                 delta_y;
        i32                 mouse_wheel_delta;
        PlatformButtonState left_button;
        struct {
            i32 x;
            i32 y;
        } raw;

    } mouse;

    struct {
        f64 time; // in seconds
        f64 delta_s;
        f64 delta_ms;
        u64 frame_count;
        struct {
            f64 time;
            f64 start_time;
        } raw;
    } timing;

    void *user_context;

#if defined(_WIN32)
    PlatformWin32 win32;
#endif

} Platform;

// "User-defined" functions
Platform p_config(void);
void     p_init(Platform *p);
void     p_frame(Platform *p);
void     p_shutdown(Platform *p);

// Platform API
void platform_print_fmt(Allocator *a, const char *fmt, ...);
void platform_print_line(const char *string);

static inline void platform_init_defaults(Platform *p) {
    if (maybe_is_nothing(p->window.width)) {
        p->window.width = maybe_some(u32, 800);
    }

    if (maybe_is_nothing(p->window.height)) {
        p->window.height = maybe_some(u32, 600);
    }

    if (maybe_is_nothing(p->window.title)) {
        p->window.title = maybe_some(charptr, "ProfusePlatformWindow");
    }

    if (maybe_is_nothing(p->window.cursor_style)) {
        p->window.cursor_style = maybe_some(PlatformCursorStyle, PLATFORM_CURSOR_STYLE_NORMAL);
    }
}

static inline void platform_button_state_set(PlatformButtonState *b, boolean value) {
    b->was_down = b->is_down;
    b->is_down = value;
    b->was_up = b->is_up;
    b->is_up = !b->is_down;
    b->just_down = b->is_down && !b->was_down;
    b->just_up = b->is_up && b->was_down;
}

#endif /* __PLATFORM_H */
