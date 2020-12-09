#include "allocators.h"
#include "common.h"
#include "error.h"
#include "result.h"

static const u32 max_alignment = sizeof(intmax_t);

static boolean is_pot(usize x)
{
    return (x & (x - 1)) == 0;
}

static Result(uptr) align_pointer_forward(uptr ptr, usize alignment)
{
    if (!is_pot(alignment)) {
        return result_error(uptr, ERR_ALLOCATORS_MEMORY_ALIGNMENT);
    }

    uptr a   = (uptr)alignment;
    uptr mod = ptr % a;

    if (mod == 0) {
        return result_ok(uptr, ptr);
    }

    return result_ok(uptr, ptr + (a - (ptr % a)));
}

//#ifdef __wasm__
// extern u8 __heap_base;
//#endif

ErrorCode allocators_linear_init(AllocatorsLinear *allocator, AllocatorsChildAllocator child_allocator,
                                 usize initial_capacity)
{
    Result(uptr) mbase = child_allocator.alloc(initial_capacity);

    if (result_is_error(mbase)) {
        return mbase.error_code;
    }

    if ((mbase.value % max_alignment) != 0) {
        return ERR_ALLOCATORS_MEMORY_ALIGNMENT;
    }

    allocator->base            = mbase.value;
    allocator->capacity        = initial_capacity;
    allocator->current_offset  = 0;
    allocator->child_allocator = child_allocator;

    return ERR_OK;
}

void allocators_linear_deinit(AllocatorsLinear *allocator)
{
    if (allocator->base) {
        allocator->child_allocator.free(allocator->base);
    }
}

Result(uptr) allocators_linear_alloc(AllocatorsLinear *allocator, usize amount)
{
    Result(uptr) result_ptr = align_pointer_forward(allocator->base + allocator->current_offset, max_alignment);

    if (result_is_error(result_ptr)) {
        return result_ptr;
    }

    uptr result = result_ptr.value;

    usize next_offset = (result - allocator->base) + amount;

    // TODO: Try to grow memory here. Maybe use ziglang arena allocator's approach of a list of buffers, since we may
    // not be able to realloc/grow in the same place on all platforms.
    if (next_offset > allocator->capacity) {
        return result_error(uptr, ERR_ALLOCATORS_LINEAR_OUT_OF_MEMORY);
    }

    allocator->current_offset = next_offset;

    return result_ok(uptr, result);
}

#ifdef __RUN_TESTS

#include "allocators.h"
#include "result.h"
#include "test.h"
#include <stdlib.h>

Result(uptr) __test_alloc(usize amount)
{
    uptr buf = (uptr)malloc(amount);

    if (!buf) {
        return result_error(uptr, ERR_OUT_OF_MEMORY);
    }

    return result_ok(uptr, buf);
}

void __test_free(uptr buf) {}

test(linear_allocator)
{
    AllocatorsLinear alloc         = { 0 };
    AllocatorsChildAllocator child = { .alloc = __test_alloc, .free = __test_free };

    allocators_linear_init(&alloc, child, 100);

    assert(alloc.capacity == 100);
    assert(alloc.current_offset == 0);

    Result(uptr) mx = allocators_linear_alloc(&alloc, 10);

    assert(result_is_ok(mx));

    uptr x = mx.value;

    assert(x == alloc.base);

    Result(uptr) aligned = allocators_linear_alloc(&alloc, 1);

    assert(result_is_ok(aligned));
    assert((aligned.value % max_alignment) == 0);

    Result(uptr) my = allocators_linear_alloc(&alloc, 100);

    assert(result_is_error(my));
}

suite()
{
    run_test(linear_allocator);
}

#endif /* __RUN_TESTS */
