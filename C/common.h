#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>
#include <string.h>

struct SubCommand {
    char *name;
    int (*run)(int argc, char *argv[]);
};

enum ReturnValue {
    EXECUTION_SUCCESS = 0,
    INVALID_PARAMETERS = 1,
    NOT_INITIALIZED = 2,
    ENV_HOME_NOT_SET = 3
};

char *todo_dir_path();
char *todo_file_path();

#endif
