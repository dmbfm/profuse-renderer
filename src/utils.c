#include <stdarg.h>
#include "common.h"
#include "format.h"

#ifndef __wasm__
#include <stdio.h>
#include <stdlib.h>
#endif /* __wasm__ */

static void debug_log_output_string(const char *string)
{
#if defined(__wasm__)
    common_wasm_print_message(string);
#elif defined(_WIN32)
#include "win32_api.h"
    OutputDebugString(string);
    OutputDebugString("\n");
#else
    printf("%d\n", string);
#endif /* defined(__wasm__) */
}

void debug_log(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    size_t len = formatv(0, 0, fmt, args);
    len++;

#ifdef _MSC_VER
    char *buf = (char *) malloc(len);
#else
    char buf[len];
#endif

    formatv(buf, len, fmt, args);
    debug_log_output_string(buf);

#ifdef _MSC_VER
    free(buf);
#endif
    va_end(args);
}
