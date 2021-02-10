#if defined(__RUN_TESTS)

#include "common.h"

static boolean aborted = false;
#undef panic
#define panic() (aborted = true)

#include "slice.h"
#include "test.h"

test(slices) {
    i32 xs[]      = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Slice(i32) s1 = slice_from_array(i32, xs, 10);
    Slice(i32) s2 = slice_from_array_range(i32, xs, 2, 8);

    assert(slice_len(s1) == 10);
    assert(slice_len(s2) == 6);

    slice_get(s1, 23);
    assert(aborted == true);
    aborted = false;

    slice_for(i, s1) {
        assert(slice_get(s1, i) == i);
    }

    slice_for(i, s2) {
        assert(slice_get(s2, i) == (i + 2));
    }

    slice_set(s1, 3, 23);
    assert(slice_get(s1, 3) == 23);
}

suite() {
    run_test(slices);
}

#endif /* __RUN_TESTS */
