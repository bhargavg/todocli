#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>
#include "item.h"

struct SubCommand {
    char *name;
    char *description;
    int (*run)(int argc, char *argv[]);
    void (*help_printer)(FILE *stream);
};

enum ReturnValue {
    EXECUTION_SUCCESS = 0,
    INVALID_ARGUMENTS = 1,
    NOT_INITIALIZED = 2,
    ENV_HOME_NOT_SET = 3,
    UNKNOWN_ERROR = 4
};

struct Argument {
    char *long_name;
    char *short_name;
    bool is_flag;
    int (*value_processor)(char *, void *);
};

char *todo_dir_path();
char *todo_file_path();

int open_todo_file_for_editing(char *, FILE **fp);
int seek_till_item_index(FILE *, int);
int seek_till_next_item(FILE *);
int update_item_status(FILE *, enum ItemStatus);
int update_item_stats_at_index(FILE *, int, enum ItemStatus);

int sanitized_index_arg_value(char *arg, int *index);

#endif
