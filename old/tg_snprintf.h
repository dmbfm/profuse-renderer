#ifndef __TG_SNPRINTF_H
#define __TG_SNPRINTF_H

#include <stddef.h>

int tg__snprintf(char *s, size_t n, const char *fmt, ...);

#define tg_snprintf tg__snprintf

#endif
