#include "libs/minunit.h"
#include "args-parser-extract-arguments-test-suite.h"

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(arg_parser_extract_arguments_test_suite);
    MU_REPORT();
    return 0;
}
