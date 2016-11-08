#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

int run_rm(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count != 1) {
        //FIXME: handle this in is_rm_args_valid
        //printf("\"rm\" takes only one argument, %d provided\n", options->values_count);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
        //FIXME: handle this in is_rm_args_valid
        // printf("error: invalid argument");
        //print_rm_help(stdout);
        return UNKNOWN_ERROR;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = REMOVED;
        return EXECUTION_SUCCESS;
    }

    return TODO_ITEM_NOT_FOUND;
}

const struct SubCommand rm_subcommand = {
    .name = "rm",
    .run = run_rm,
    .help_text = "Remove the specified todo item(s)\n\n"
                 "Usage: todo rm [ITEM_ID] [--pending] [--completed] [--dir DIRECTORY_PATH]\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo is initialized\n"
                 "  -p, --pending\n"
                 "      Remove all pending items\n"
                 "  -c, --completed\n"
                 "      Remove all completed items\n"
};
