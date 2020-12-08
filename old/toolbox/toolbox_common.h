#if !defined(TOOLBOX_TYPES_H)
#define TOOLBOX_TYPES_H

#include <stdint.h>
#include <stddef.h>

#define global static
#define local_function static

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned int uint;
typedef unsigned char uchar;

typedef intptr_t iptr;
typedef uintptr_t uptr;

typedef float f32;
typedef double f64;

typedef u8 boolean;

#define true ((boolean)1);
#define false ((boolean)0);

#if defined(_MSC_VER)
#define TOOLBOX_DEBUG_BREAK __debugbreak()
#elif defined(__GNUC__)
#define TOOLBOX_DEBUG_BREAK __builtin_trap()
#elif defined(__clang__)
#define TOOLBOX_DEBUG_BREAK __builtin_debugtrap()
#else
#define TOOLBOX_DEBUG_BREAK \
    {                  \
        *(int *)0 = 0; \
    }
#endif

#ifdef TOOLBOX_DEBUG
#define t_assert(exp) \
    if (!(exp)) {            \
        TOOLBOX_DEBUG_BREAK;      \
    }
#else 
#define t_assert(exp)
#endif



#endif
