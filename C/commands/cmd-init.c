#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "cmd-init.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

struct SubCommandExecResult *run_init(struct Options *options, struct TodoListMetadata *metadata) {

    struct SubCommandExecResult *result = exec_result_new();

    if (options->values_count > 0) {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "\"init\" takes 0 arguments, %d provided", options->values_count);
        return result;
    }

    result->status = initialize(options->dir_path);
    result->message = strdup("Initialized");
    return result;
};


const struct SubCommand init_subcommand = {
    .name = "init",
    .run = run_init,
    .help_text = "Initialize todo\n\n"
                 "Usage: todo init [--dir DIRECTORY_PATH]\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo should be initialized\n"
};
