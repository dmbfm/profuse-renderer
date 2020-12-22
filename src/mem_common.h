#ifndef __MEM_COMMON_H
#define __MEM_COMMON_H

#include "common.h"
#include "result.h"
#include <stdint.h>

enum
{
    mem_max_alignment = sizeof(intmax_t)
};

static inline boolean mem_is_pot(usize x)
{
    return (x & (x - 1)) == 0;
}

static inline boolean mem_is_aligned(uptr ptr, usize alignment)
{
    uptr a   = (uptr)alignment;
    uptr mod = ptr % a;

    return (mod == 0);
}

static inline Result(uptr) mem_align_pointer_forward(uptr ptr, usize alignment)
{
    if (!mem_is_pot(alignment)) {
        return result_error(uptr, ERR_ALLOCATORS_MEMORY_ALIGNMENT);
    }

    uptr a   = (uptr)alignment;
    uptr mod = ptr % a;

    if (mod == 0) {
        return result_ok(uptr, ptr);
    }

    return result_ok(uptr, ptr + (a - (ptr % a)));
}

static inline Result(uptr) mem_aling_pointer_back(uptr ptr, usize alignment)
{
    if (!mem_is_pot(alignment)) {
        return result_error(uptr, ERR_ALLOCATORS_MEMORY_ALIGNMENT);
    }

    uptr a   = (uptr)alignment;
    uptr mod = ptr % a;

    if (mod == 0) {
        return result_ok(uptr, ptr);
    }

    return result_ok(uptr, ptr - mod);
}

#endif /* __MEM_COMMON_H */
