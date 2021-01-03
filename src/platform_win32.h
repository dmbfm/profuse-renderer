#ifndef __PLATFORM_WIN32_H
#define __PLATFORM_WIN32_H 

#ifdef _WIN32

#include "common.h"
#include "win32_api.h"

typedef struct PlatformWin32
{
    HWND window_handle;
    HDC device_context;

    i32 mouse_x_raw;
    i32 mouse_y_raw;
} PlatformWin32;

#endif /* _WIN32 */
#endif /* __PLATFORM_WIN32_H */
