#include "coffee.h"

#if defined(_WIN32)
#include "coffee_win32.c"
#elif defined(__wasm__)
#include "coffee_web.c"
#endif
