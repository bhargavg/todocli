#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-tick.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

struct SubCommandExecResult *run_tick(struct Options *options, struct TodoListMetadata *metadata) {
    struct SubCommandExecResult *result = exec_result_new();

    if (options->values_count != 1) {
        result->status = NO_ERROR;
        asprintf(&(result->message), "\"tick\" takes only one argument, %d provided\n", options->values_count);

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
        item->status = COMPLETED;

        result->status = NO_ERROR;
        result->message = NULL;
    } else {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "couldnot find item with identifier: %lu", identifier);
    }

    return result;
};

const struct SubCommand tick_subcommand = {
    .name = "tick",
    .run = run_tick,
    .help_text = "Mark item with specified id as completed\n\n"
                 "Usage: todo tick ITEM_ID\n\n"
                 "Options: \n"
                 "   -d, --dir\n"
                 "       Path to directory in which todo is initialized\n"
};

