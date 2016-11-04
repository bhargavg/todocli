#include <stdio.h>
#include "../argument-parser.h"

MU_TEST(test_extract_values) {
    struct ArgParserResult *result = NULL;
    char *input_args[] = { "foo", "bar", "baz" };
    extract_arguments(3, input_args, &result);

    mu_assert(result->values_count == 3, "Values count is not 3");
    mu_assert(strcmp(result->values[0], "foo") == 0, "0th value is not \"foo\"");
    mu_assert(strcmp(result->values[1], "bar") == 0, "1st value is not \"bar\"");
    mu_assert(strcmp(result->values[2], "baz") == 0, "2nd value is not \"baz\"");

    free_arg_parser_result(result);
}

MU_TEST(test_extract_values) {
    struct ArgParserResult *result = NULL;
    char *input_args[] = { "--foo", "value1", "-b", "value2", "-bar", "-baz", "value3" };
    extract_arguments(3, input_args, &result);

    mu_assert(result->values_count == 3, "Values count is not 3");
    mu_assert(strcmp(result->values[0], "foo") == 0, "0th value is not \"foo\"");
    mu_assert(strcmp(result->values[1], "bar") == 0, "1st value is not \"bar\"");
    mu_assert(strcmp(result->values[2], "baz") == 0, "2nd value is not \"baz\"");

    free_arg_parser_result(result);
}

MU_TEST_SUITE(arg_parser_extract_arguments_test_suite) {
    MU_SUITE_CONFIGURE(NULL, NULL);
    MU_RUN_TEST(test_extract_values);
    MU_RUN_TEST(test_extract_key_values);
    //MU_RUN_TEST(test_extract_mixed);
}
