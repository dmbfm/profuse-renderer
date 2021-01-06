#if defined(__RUN_TESTS)

#include "list.h"
#include "heap.h"
#include "test.h"

// TODO: Right now we need to do this hacky undef to include dependencies... fix so that all dependencies
// are defined in build.py instead, like they should.
#undef __RUN_TESTS
#include "heap.c"

test(list_push)
{
    i32 *xs = list_new(i32, &heap_allocator);

    forn(i, 1000)
    {
        list_push(xs, i);
    }

    list_for(i, xs)
    {
        expect(xs[i] == i);
    }

    list_free(xs);

    expect(true);
}

test(list_as_slice)
{
    i32 *xs = list_new(i32, &heap_allocator);

    forn(i, 1000)
    {
        list_push(xs, i);
    }

    Slice(i32) s = list_as_slice(i32, xs);

    slice_for(i, s) {
        expect(slice_get(s, i) == i);
    }

    list_free(xs);

    expect(true);
}

suite()
{
    run_test(list_push);
    run_test(list_as_slice);
}

#endif /* defined(__RUN_TESTS) */
