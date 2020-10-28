#define __TG_RUN_TESTS

#ifndef __wasm__
#include <stdio.h>
#include <stdlib.h>
#endif

#include "tg_wasm.h"
#include "tg_debug.h"
#include "tg_platform.h"
#include "tg.h"
#include "tg_math.h"

export int main()
{
#ifdef __TG_RUN_TESTS
    tg_h_test();
    tg_math_test();
#endif
}

