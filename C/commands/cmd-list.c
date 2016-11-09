#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

char *all_items_as_string(struct TodoListMetadata metadata);
char *items_with_status_as_string(struct TodoListMetadata metadata, enum ItemStatus status);
char *item_as_string(struct TodoItem item);
char *summary(struct TodoListMetadata metadata);

struct SubCommandExecResult *run_list(struct Options *options, struct TodoListMetadata *metadata) {
    struct SubCommandExecResult *result = exec_result_new();

    if (options->flags == OPTIONS_FLAG_PENDING) {
        result->message = items_with_status_as_string(*metadata, NOT_COMPLETED);
    } else if (options->flags == OPTIONS_FLAG_COMPLETED) {
        result->message = items_with_status_as_string(*metadata, COMPLETED);
    } else if (options->flags == OPTIONS_FLAG_SUMMARY) {
        result->message = summary(*metadata);
    } else if (options->flags == OPTIONS_FLAG_ALL || options->flags == OPTIONS_FLAG_UNSPECIFIED) {
        result->message = all_items_as_string(*metadata);
    } else {
        result->status = MULTIPLE_FLAGS;
        result->message = strdup("multiple flags specified");
    }

    return result;
};

char *summary(struct TodoListMetadata metadata) {
    unsigned long int pending_items_count = 0, completed_items_count = 0;
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];
        if (item->status == COMPLETED) {
            completed_items_count++;
        } else if (item->status == NOT_COMPLETED) {
            pending_items_count++;
        }
    }

    char *message = NULL;
    asprintf(&message, "You have %lu pending item(s) and completed %lu item(s)\n", pending_items_count, completed_items_count);

    return message;
}

char *all_items_as_string(struct TodoListMetadata metadata) {
    bool printed_any = false;
    char *message = NULL;
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];
        asprintf(&message, "%s%s\n", (message) ?: "", item_as_string(*item));
        printed_any = true;
    }

    if (!printed_any) {
        asprintf(&message, "You have 0 items\n");
    }

    return message;
}

char *items_with_status_as_string(struct TodoListMetadata metadata, enum ItemStatus status) {
    bool printed_any = false;
    char *message = NULL;
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];

        if (item->status == status) {
            asprintf(&message, "%s%s\n", (message) ?: "", item_as_string(*item));
            printed_any = true;
        }
    }

    if (!printed_any) {
        asprintf(&message, "You have 0 %s items\n", (status == COMPLETED) ? "completed" : "pending");
    }

    return message;
}

char *item_as_string(struct TodoItem item) {
    char *status = (item.status == COMPLETED) ? "✔" : ((item.status == REMOVED) ? "✘" : " ");

    char *message = NULL;
    asprintf(&message, "%s %lu: %s", status, item.identifier, item.text);

    return message;
}


const struct SubCommand list_subcommand = {
    .name = "list",
    .run = run_list,
    .help_text = "Print the todo items\n\n"
                 "Usage: todo list [--summary] [--pending] [--completed]\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo is initialized\n"
                 "  -p, --pending\n"
                 "      List all pending items\n"
                 "  -c, --completed\n"
                 "      List all completed items\n"
                 "  -s, --summary\n"
                 "      Print one line summary of completed/pending items\n",
};
