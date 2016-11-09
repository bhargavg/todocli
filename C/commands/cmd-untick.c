#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-untick.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

struct SubCommandExecResult *run_untick(struct Options *options, struct TodoListMetadata *metadata) {
    struct SubCommandExecResult *result = exec_result_new();

    if (options->values_count != 1) {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "\"untick\" takes only one argument, %d provided\n", options->values_count);
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
        if (item->status == NOT_COMPLETED) { 
            result->status = INVALID_ARGUMENT;
            result->message = strdup("Item is not yet marked as completed!");
        } else {
            item->status = NOT_COMPLETED;

            result->status = NO_ERROR;
            result->message = strdup("Get back to work, this item is marked as incomplete");
        }

    } else {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "Couldnot find item with identifier: %lu", identifier);
    }

    return result;
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

