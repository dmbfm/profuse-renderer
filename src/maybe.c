#include "maybe.h"

#if 1 //#ifdef __RUN_TESTS
#include "test.h"

i32 __test_inc(i32 x)
{
    return x + 1;
}

i32 __test_add(i32 x, i32 y)
{
    return x + y;
}

test(maybe) {
    maybe(i32) x = maybe_some(i32, 10);
    maybe(i32) y = maybe_nothing(i32);

    assert(maybe_is_something(x) == true);
    assert(!maybe_is_nothing(x));
    assert(x.value == 10);

    assert(maybe_is_nothing(y))
    assert(!maybe_is_something(y))
    assert(maybe_is_nothing(maybe_map(i32, y, __test_inc)))
    assert(maybe_is_something(maybe_map(i32, x, __test_inc)))

    maybe(i32) z = maybe_map(i32, x, __test_inc);
    assert(z.value == 11);

    maybe(i32) w = maybe_lift(i32, x, y, __test_add);
    assert(maybe_is_nothing(w));

    maybe(i32) k = maybe_lift(i32, x, z, __test_add);
    assert(maybe_is_something(k));
    assert(k.value == 21);

    maybe_ifsome(y) {
        assert(false);
    }

    maybe_set_some(y, 10);
    maybe_ifsome(y) {
        assert(y.value == 10);
    }

    maybe_set_nothing(x);
    maybe_ifsome(x) {
        assert(false);
    }
}

suite() {
    run_test(maybe);
}

#endif /* __RUN_TESTS */

