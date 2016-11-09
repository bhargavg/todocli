#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

unsigned long int remove_all_items(struct TodoListMetadata *metadata);
bool remove_item_with_identifier(unsigned long int identifier, struct TodoListMetadata *metadata);
unsigned long int remove_items_with_status(enum ItemStatus status, struct TodoListMetadata *metadata);

struct SubCommandExecResult *run_rm(struct Options *options, struct TodoListMetadata *metadata) {
    struct SubCommandExecResult *result = exec_result_new();

    if (options->flags == OPTIONS_FLAG_PENDING) {
        unsigned long int removed_items_count = remove_items_with_status(NOT_COMPLETED, metadata);

        result->status = NO_ERROR;
        asprintf(&(result->message), "Removed %lu items", removed_items_count);

    } else if (options->flags == OPTIONS_FLAG_COMPLETED) {
        unsigned long int removed_items_count = remove_items_with_status(COMPLETED, metadata);

        result->status = NO_ERROR;
        asprintf(&(result->message), "Removed %lu items", removed_items_count);

    } else if (options->flags == OPTIONS_FLAG_ALL) {
        unsigned long int removed_items_count = remove_all_items(metadata);

        result->status = NO_ERROR;
        asprintf(&(result->message), "Removed %lu items", removed_items_count);

    } else if (options->flags == OPTIONS_FLAG_UNSPECIFIED) {
        if (options->values_count != 1) {
            result->status = INVALID_ARGUMENT;
            asprintf(&(result->message), "\"rm\" takes only one argument, %d provided\n", options->values_count);
            return result;
        }

        unsigned long int identifier = 0;
        if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
            result->status = INVALID_ARGUMENT;
            result->message = strdup("Item id should be integer");
            return result;
        }

        if (remove_item_with_identifier(identifier, metadata)) {
            result->status = NO_ERROR;
            result->message = strdup("Removed the item");
        } else {
            result->status = INVALID_ARGUMENT;
            result->message = strdup("Could not find item");
        }

    } else {
        result->status = MULTIPLE_FLAGS;
        result->message = strdup("multiple flags specified");
    }

    return result;
}

unsigned long int remove_items_with_status(enum ItemStatus status, struct TodoListMetadata *metadata) {
    unsigned long int removed_items_count = 0;

    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];
        if (item->status == status && item->status != REMOVED) {
            item->status = REMOVED;
            removed_items_count++;
        }
    }

    return removed_items_count;
}

unsigned long int remove_all_items(struct TodoListMetadata *metadata) {
    unsigned long int removed_items_count = 0;

    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];
        if (item->status != REMOVED) {
            item->status = REMOVED;
            removed_items_count++;
        }
    }

    return removed_items_count;
}

bool remove_item_with_identifier(unsigned long int identifier, struct TodoListMetadata *metadata) {
    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];
        if (item->identifier == identifier && item->status != REMOVED) {
            item->status = REMOVED;
            return true;
        }
    }

    return false;
}

const struct SubCommand rm_subcommand = {
    .name = "rm",
    .run = run_rm,
    .help_text = "Remove the specified todo item(s)\n\n"
                 "Usage: todo rm [ITEM_ID] [--pending] [--completed] [--dir DIRECTORY_PATH]\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo is initialized\n"
                 "  -a, --all\n"
                 "      Remove all items\n"
                 "  -p, --pending\n"
                 "      Remove all pending items\n"
                 "  -c, --completed\n"
                 "      Remove all completed items\n",
};
