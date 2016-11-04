#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>
#include "libs/libtodo/libtodo.h"

struct SubCommand {
    char *name;
    char *description;
    int (*run)(int argc, char *argv[], struct TodoListMetadata *metadata);
    void (*help_printer)(FILE *stream);
};

struct Context {
    char *dir_path;
    char *file_path;
    struct TodoListMetadata *metadata;
    int argc;
    char *argv[];
};

char *get_default_todo_directory();
void die(int status, char *message);

int sanitized_index_arg_value(char *arg, unsigned long int *index);

bool has_prefix(const char *str, const char *pre);

#endif
