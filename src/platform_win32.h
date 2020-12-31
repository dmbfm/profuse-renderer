#ifndef __PLATFORM_WIN32_H
#define __PLATFORM_WIN32_H 

#ifdef _WIN32
#include <Windows.h>

typedef struct PlatformWin32
{
    HWND window_handle;
    HDC device_context;
} PlatformWin32;

#endif /* _WIN32 */
#endif /* __PLATFORM_WIN32_H */
