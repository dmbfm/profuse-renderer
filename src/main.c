#define __TG_RUN_TESTS
#include "tg_wasm.h"

#ifndef __wasm__
#include <stdio.h>
#define tg_snprinf snprintf
#define tg_printf printf
#endif

#include "tg_debug.h"
#include "tg.h"
#include "tg_math.h"

export int main()
{
#ifdef __TG_RUN_TESTS
    tg_h_test();
    tg_math_test();
#endif
}
