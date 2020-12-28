#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "common.h"
#include "platform.h"

static Platform platform;

void platform_print_line(const char *string)
{
    OutputDebugStringA(string);
    OutputDebugStringA("\n");
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNUSED_VARIABLE(hInstance);
    UNUSED_VARIABLE(hPrevInstance);
    UNUSED_VARIABLE(lpCmdLine);
    UNUSED_VARIABLE(nShowCmd);

    platform = p_config();

    p_init(&platform);

    while(!platform.should_quit) {
        platform.timing.counter++;
        p_frame(&platform);
    }

    p_shutdown(&platform);

    return 1;
}

