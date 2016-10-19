#include "lib/minunit.h"

void test_setup() {
}

void test_teardown() {
}

MU_TEST(test_check) {
    mu_check(5 == 5);
}

MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
