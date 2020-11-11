#if !defined(COFFEE_H)
#define COFFEE_H

#include "toolbox_types.h"

#ifdef __wasm__
#include "coffee_web.h"
#endif

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

// These are user-defined
int coffee_main(int argc, char **argv);
void coffee_frame(Coffee *c);
void coffee_shutdown(Coffee *c);

boolean coffee_init(Coffee *c);
void coffee_run(Coffee *c);
void coffee_pull(Coffee *c);
void coffee_push(Coffee *c);

#endif
