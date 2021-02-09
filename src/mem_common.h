#ifndef __MEM_COMMON_H
#define __MEM_COMMON_H

#include "common.h"
#include "result.h"
#include <stdint.h>

enum { mem_max_alignment = sizeof(intmax_t) };

static inline boolean mem_is_pot(usize x) {
    return (x & (x - 1)) == 0;
}

static inline boolean mem_is_aligned(uptr  ptr,
                                     usize alignment) {
    uptr a   = (uptr)alignment;
    uptr mod = ptr % a;

    return (mod == 0);
}

static inline Result(uptr)
    mem_align_pointer_forward(uptr ptr, usize alignment) {
    if (!mem_is_pot(alignment)) {
        return result_error(
            uptr,
            ERR_ALLOCATORS_MEMORY_ALIGNMENT);
    }

    usize mask    = (alignment - 1);
    uptr  aligned = (ptr + mask) & (~mask);

    /* Why this works:
     *  MASK = A - 1
     *  X + MASK =  X + (A - 1)
     *
     *  Take for instance, A = 8, i.e., A = 0000 1000.  Then
     * MASK = 0000 0111, and ~MASK = 1111 1000 X + MASK = X
     * + 7. Then (X + 7) & (1111 1000) means we are always
     * taking (X + 7) in multiples of 8, hence this always
     * returns the next multiple of 8 after X.
     */

    return result_ok(uptr, aligned);
}

#endif /* __MEM_COMMON_H */
