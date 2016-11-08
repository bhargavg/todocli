#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

struct SubCommandExecResult *run_rm(struct Options *options, struct TodoListMetadata *metadata) {
    struct SubCommandExecResult *result = exec_result_new();

    if (options->values_count != 1) {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "\"rm\" takes only one argument, %d provided\n", options->values_count);
        return result;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
        result->status = INVALID_ARGUMENT;
        result->message = strdup("invalid item id");
        return result;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = REMOVED;

        result->status = NO_ERROR;
        result->message = NULL;
    } else {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "couldnot find item with identifier: %lu", identifier);
    }

    return result;
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
                 "      Remove all completed items\n",
};
