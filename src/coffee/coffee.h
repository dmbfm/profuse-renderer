#if !defined(COFFEE_H)
#define COFFEE_H

#include "toolbox.h"

typedef struct
{
    boolean should_quit;

    struct
    {
        char *title;

        u32 x;
        u32 y;
        u32 width;
        u32 height;

        boolean resized;

    } window;

    struct
    {
        int x;
        int y;
    } mouse;

    void *user_context;
} Coffee;


#endif
