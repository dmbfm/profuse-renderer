#ifndef __TEST_H
#define __TEST_H

//#if defined(__RUN_TESTS)
#include <stdio.h>
#include <stdlib.h>

#define test(name) void test_##name()
#define suite()    void __suite()
#define run_test(name)                                                                                                 \
    printf("Testing '%s'... ", #name);                                                                                 \
    test_##name();                                                                                                     \
    printf("OK\n");
#define expect(exp)                                                                                                    \
    if (!(exp)) {                                                                                                      \
        printf("FAIL! Expected that '%s' at '%s:%d'. \n", #exp, __FILE__, __LINE__);                                   \
        exit(-1);                                                                                                      \
    }

void __suite(void);

int main(int argc, char **argv) {
    __suite();
}

//#endif /* __RUN_TESTS */

#endif /* __TEST_H */
