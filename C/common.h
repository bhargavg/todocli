#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>
#include "libs/libtodo/libtodo.h"
#include "argument-parser.h"

struct SubCommand {
    char *name;
    char *description;
    int (*run)(struct Options *options, struct TodoListMetadata *metadata);
    void (*help_printer)(FILE *stream);
};

char *get_default_todo_directory();
void die(int status, char *message);

int sanitized_index_arg_value(char *arg, unsigned long int *index);

bool has_prefix(const char *str, const char *pre);
bool is_param(char *);

#endif
