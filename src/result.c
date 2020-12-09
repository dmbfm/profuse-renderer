#ifdef __RUN_TESTS

#include "result.h"
#include "test.h"

test(result) {
    Result(i32) x = result_ok(i32, 10);
    Result(i32) y = result_error(i32, ERR_UNKNOWN);

    assert(result_is_ok(x) == true);
    assert(result_is_error(x) == false);
    assert(x.value == 10);

    assert(result_is_ok(y) == false);
    assert(result_is_error(y) == true);
    assert(y.error_code == ERR_UNKNOWN);

    result_ifok(y) {
        assert(false);
    }

    result_set_ok(y, 20);
    assert(result_is_ok(y) == true);
    assert(y.value == 20);

    result_set_error(x, ERR_OUT_OF_MEMORY);
    assert(result_is_error(x) == true);
    assert(x.error_code == ERR_OUT_OF_MEMORY);
}

suite() {
    run_test(result);
}

#endif /* __RUN_TESTS */
