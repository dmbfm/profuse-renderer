#include "platform.h"

Platform p_config()
{
    Platform p = { 0 };

    return p;
}

void p_init(Platform *p) 
{
    platform_print_line("p_init");
}

void p_frame(Platform *p)
{
    platform_print_line("p_frame");

    if (p->timing.counter >= 10) p->should_quit = true;
}

void p_shutdown(Platform *p)
{
    platform_print_line("p_shutdown");
}
