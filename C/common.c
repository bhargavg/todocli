#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "common.h"
#include "commands/cmd-init.h"

int sanitized_index_arg_value(char *arg, unsigned long int *index) {
    unsigned long int value = strtoul(arg, NULL, 10);
   *index = value;
   return EXECUTION_SUCCESS;
}

// From: http://stackoverflow.com/a/4770992
bool has_prefix(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int process_arguments(int argc, char *argv[], int allowed_args_count, struct Argument allowed_args[], void *options_bag, struct Argument values[], int *values_count, char *invalid_args[], int *invalid_args_count) {

    *values_count = 0;
    *invalid_args_count = 0;

    //FIXME: O(n^2), optimize this
    for (int i = 0; i < argc; i++) {
        char *input_arg_string = argv[i];
        bool arg_found = false;

        if (!has_prefix(input_arg_string, "-")) {
            // this is value
            struct Argument value = { .long_name = NULL, 
                                      .short_name = NULL, 
                                      .type = VALUE, 
                                      .value_processor = NULL };

            values[(*values_count)++] = value;
            continue;
        }

        for (int j = 0; j < allowed_args_count; j++) {
            struct Argument arg = allowed_args[j];

            if (strcmp(input_arg_string, arg.long_name) == 0
                || strcmp(input_arg_string, arg.short_name) == 0) {

                arg_found = true;

                char *value = NULL;

                if (arg.type != FLAG) {
                    // read the argument value
                    i++;
                    value = (i < argc) ? argv[i] : NULL;
                }

                if (arg.value_processor(value, options_bag) != EXECUTION_SUCCESS) {
                    invalid_args[(*invalid_args_count)++] = input_arg_string;
                }

                break;
            }
        }

        if (!arg_found) {
            invalid_args[(*invalid_args_count)++] = input_arg_string;
        }
    }

    return (*invalid_args_count > 0) ? UNKNOWN_ERROR : EXECUTION_SUCCESS;
}

