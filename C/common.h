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

char *todo_dir_path();
char *todo_file_path();

int open_todo_file_for_editing(char *, FILE **fp);
int seek_till_item_index(FILE *, int);
int seek_till_next_item(FILE *);
int update_item_status(FILE *, enum ItemStatus);
int update_item_stats_at_index(FILE *, int, enum ItemStatus);

int sanitized_index_arg_value(char *arg, unsigned long int *index);


int process_arguments(int argc, char *argv[], int allowed_args_count, struct Argument allowed_args[], void *options_bag, struct Argument values[], int *values_count, char *invalid_args[], int *invalid_args_count);

#endif
