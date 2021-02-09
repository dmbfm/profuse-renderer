#include "maybe.h"

#ifdef __RUN_TESTS
    #include "test.h"

i32 __test_inc(i32 x) {
    return x + 1;
}

i32 __test_add(i32 x, i32 y) {
    return x + y;
}

test(maybe) {
    Maybe(i32) x = maybe_some(i32, 10);
    Maybe(i32) y = maybe_nothing(i32);

    expect(maybe_is_something(x) == true);
    expect(!maybe_is_nothing(x));
    expect(x.value == 10);

    expect(maybe_is_nothing(y))
        expect(!maybe_is_something(y)) expect(
            maybe_is_nothing(maybe_map(i32, y, __test_inc)))
            expect(maybe_is_something(
                maybe_map(i32, x, __test_inc)))

                Maybe(i32) z =
                    maybe_map(i32, x, __test_inc);
    expect(z.value == 11);

    Maybe(i32) w = maybe_lift(i32, x, y, __test_add);
    expect(maybe_is_nothing(w));

    Maybe(i32) k = maybe_lift(i32, x, z, __test_add);
    expect(maybe_is_something(k));
    expect(k.value == 21);

    maybe_ifsome(y) {
        expect(false);
    }

    maybe_set_some(y, 10);
    maybe_ifsome(y) {
        expect(y.value == 10);
    }

    maybe_set_nothing(x);
    maybe_ifsome(x) {
        expect(false);
    }
}

suite() {
    run_test(maybe);
}

#endif /* __RUN_TESTS */
