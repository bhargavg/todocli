#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>
#include "libtodo.h"

struct SubCommand {
    char *name;
    char *description;
    int (*run)(int argc, char *argv[], struct TodoListMetadata *metadata);
    void (*help_printer)(FILE *stream);
};

enum ArgumentType {
    VALUE = 0,
    FLAG = 1,
    KEY_VALUE = 2
};

struct Argument {
    char *long_name;
    char *short_name;
    enum ArgumentType type;
    int (*value_processor)(char *, void *);
};

int sanitized_index_arg_value(char *arg, unsigned long int *index);


int process_arguments(int argc, char *argv[], int allowed_args_count, struct Argument allowed_args[], void *options_bag, struct Argument values[], int *values_count, char *invalid_args[], int *invalid_args_count);

#endif
