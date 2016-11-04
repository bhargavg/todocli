#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "argument-parser.h"
#include "common.h"
#include "options.h"

void process_args(int start_index, int argc, char *argv[], struct Argument args[], int args_count, struct Options *options) {
    for (int i = start_index; i < argc; i++) {
        char *arg_key = argv[i];

        if (is_param(arg_key)) {
            for (int j = 0; j < args_count; j++) {
                struct Argument arg = args[j];

                if (strcmp(arg_key, arg.long_name) == 0
                    || strcmp(arg_key, arg.short_name) == 0) {

                    if (arg.is_flag) {
                        arg.parser(NULL, options);
                    } else {
                        i++;
                        if (i < argc) {
                            char *arg_value = argv[i];
                            arg.parser(arg_value, options);
                        } else {
                            printf("Value not found for %s", arg_key);
                        }
                    }

                    continue;
                }
            }
        } else {
            options->values[options->values_count++] = strdup(arg_key);
        }
    }
}
