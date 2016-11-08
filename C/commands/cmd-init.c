#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "cmd-init.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

int run_init(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count > 0) {
        //FIXME: handle this in is_init_args_valid
        //printf("\"init\" doesnot take any parameters\n");
        return UNKNOWN_ERROR;
    }

    return initialize(options->dir_path);
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
