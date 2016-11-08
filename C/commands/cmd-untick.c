#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-untick.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

int run_untick(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count != 1) {
        //FIXME: handle this in is_untick_args_valid
        //printf("\"untick\" takes only one argument, %d provided\n", options->values_count);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
        //FIXME: handle this in is_untick_args_valid
        //printf("error: invalid argument");
        return UNKNOWN_ERROR;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = NOT_COMPLETED;
        return EXECUTION_SUCCESS;
    }

    return TODO_ITEM_NOT_FOUND;
};

const struct SubCommand untick_subcommand = {
    .name = "untick",
    .run = run_untick,
    .help_text = "Mark item with specified id as incomplete\n\n"
                 "Usage: todo untick ITEM_ID\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo is initialized\n"
};

