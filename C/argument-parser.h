#ifndef _ARGUMENT_PARSER_H__
#define _ARGUMENT_PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "options.h"

struct Argument {
    char *long_name;
    char *short_name;
    bool is_flag;
    int (*parser) (char *value, struct Options *);
    bool (*is_valid)(char *);
};

void process_args(int start_index, int argc, char *argv[], struct Argument args[], int args_count, struct Options *options);

#endif
