#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>
#include "libs/libtodo/libtodo.h"
#include "argument-parser.h"

struct SubCommand {
    const char *name;
    int (*run)(struct Options *options, struct TodoListMetadata *metadata);
    const char *help_text;
};

enum Errors {
    NO_ERROR = 0,
    INVALID_ARGUMENT,
    VALUE_NOT_FOUND,
};

char *get_default_todo_directory();
void die(int status, char *message);

int sanitized_index_arg_value(char *arg, unsigned long int *index);

bool has_prefix(const char *str, const char *pre);
bool is_param(char *);

#endif
