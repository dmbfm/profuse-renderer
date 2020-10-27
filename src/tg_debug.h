#if !defined(_TG_DEBUG_H)
#define _TG_DEBUG_H

#if defined(_MSC_VER)
#define TG_DEBUG_BREAK __debugbreak()
#elif defined(__GNUC__)
#define TG_DEBUG_BREAK __builtin_trap()
#elif defined(__clang__)
#define TG_DEBUG_BREAK __builtin_debugtrap()
#else
#define TG_DEBUG_BREAK \
    {                  \
        *(int *)0 = 0; \
    }
#endif

#define tg__assert(exp) \
    if (!(exp)) {            \
        TG_DEBUG_BREAK;      \
    }

#define tg_assert tg__assert

#endif
