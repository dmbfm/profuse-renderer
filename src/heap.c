#include "heap.h"
#include "common.h"

#if defined(__wasm__)
#include "heap_wasm.c"
#else
#include <stdlib.h>

static Result(uptr) alloc(Allocator *alloc, usize amount)
{
    UNUSED_VARIABLE(alloc);

    uptr p = (uptr) malloc(amount);

    if (!p) return result_error(uptr, ERR_OUT_OF_MEMORY);

    return result_ok(uptr, p);
}

static Result(usize) resize(struct Allocator *allocator,  uptr region, usize new_amount, usize alignment)
{
    UNUSED_VARIABLE(allocator);
    UNUSED_VARIABLE(alignment);

    if (new_amount == 0) {
        free((void *) region);
        return result_ok(usize, 0);
    }

    return result_error(usize, ERR_OUT_OF_MEMORY);
}

Allocator heap_allocator = { .alloc = alloc, .allocator_state = 0 };

#endif /* __wasm__ */


#ifdef __RUN_TESTS

#include "test.h"

test(heap_allocator) {

}

suite() {
    run_test(heap_allocator);
}

#endif /* __RUN_TESTS */
