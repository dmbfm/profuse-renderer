#include "heap.h"
#include "common.h"

#if defined(__wasm__)
    #include "heap_wasm.c"
#else
    #include <stdlib.h>

static Result(uptr)
    heap_alloc(Allocator *alloc, usize amount) {
    UNUSED_VARIABLE(alloc);

    uptr p = (uptr)malloc(amount);

    if (!p) return result_error(uptr, ERR_OUT_OF_MEMORY);

    return result_ok(uptr, p);
}

static Result(uptr) heap_realloc(Allocator *alloc,
                                 uptr       region,
                                 usize      new_size) {
    UNUSED_VARIABLE(alloc);

    uptr p = (uptr)realloc((void *)region, new_size);

    if (!p) return result_error(uptr, ERR_OUT_OF_MEMORY);

    return result_ok(uptr, p);
}

static void heap_free(struct Allocator *allocator,
                      uptr              region) {
    UNUSED_VARIABLE(allocator);

    free((void *)region);
}

Allocator heap_allocator = {.alloc           = heap_alloc,
                            .realloc         = heap_realloc,
                            .free            = heap_free,
                            .allocator_state = 0};

#endif /* __wasm__ */

#ifdef __RUN_TESTS

    #include "test.h"

test(heap_allocator) {
}

suite() {
    run_test(heap_allocator);
}

#endif /* __RUN_TESTS */
