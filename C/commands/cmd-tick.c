#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-tick.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

int run_tick(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count != 1) {
        //FIXME: handle this in is_tick_args_valid
        // printf("\"tick\" takes only one argument, %d provided\n", options->values_count);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
        //FIXME: handle this in is_tick_args_valid
        //printf("error: invalid argument");
        return UNKNOWN_ERROR;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = COMPLETED;
        return EXECUTION_SUCCESS;
    }

    return TODO_ITEM_NOT_FOUND;
};

const struct SubCommand tick_subcommand = {
    .name = "tick",
    .run = run_tick,
    .help_text = "      Usage: todo tick ITEM_ID\n"
                 "Description: Mark item with specified id as completed\n"
                 "    Options: \n"
                 "      -d, --dir   Path to directory in which todo is initialized\n\n"
                 "    Example: todo tick 2\n"
};

