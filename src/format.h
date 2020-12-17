#ifndef __FORMAT_H

#include "common.h"
#include <stdarg.h>

int format(char *s, usize n, const char *fmt, ...);
int formatv(char *s, usize n, const char *fmt, va_list arg);

#define __FORMAT_H 
#endif /* __FORMAT_H */
