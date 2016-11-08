#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "argument-parser.h"
#include "common.h"
#include "options.h"

int process_args(int start_index, int argc, char *argv[], struct Argument args[], int args_count, struct Options *options, char **invalid_argument) {
    for (int i = start_index; i < argc; i++) {
        char *arg_key = argv[i];

        if (is_param(arg_key)) {
            enum Errors error = NO_ERROR;
            bool arg_found = false;
            for (int j = 0; j < args_count; j++) {
                struct Argument arg = args[j];

                if (strcmp(arg_key, arg.long_name) == 0
                    || strcmp(arg_key, arg.short_name) == 0) {

                    arg_found = true;

                    if (arg.is_flag) {
                        arg.parser(NULL, options);
                        break;
                    } else {
                        i++;
                        if (i < argc) {
                            char *arg_value = argv[i];
                            arg.parser(arg_value, options);
                            error = NO_ERROR;
                            break;
                        } else {
                            // Cannot retrieve value for this argument
                            error = ARG_VALUE_NOT_FOUND;
                        }
                    }

                    continue;
                }
            }

            if (!arg_found) {
                error = INVALID_ARGUMENT;
            }

            if (error) {
                *invalid_argument = strdup(arg_key);
                return error;
            }

        } else {
            options->values[options->values_count++] = strdup(arg_key);
        }
    }

    return EXECUTION_SUCCESS;
}
