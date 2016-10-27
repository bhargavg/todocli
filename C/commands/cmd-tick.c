#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-tick.h"
#include "cmd-init.h"
#include "../common.h"
#include "libtodo.h"

void print_tick_help(FILE *);

int run_tick(int argc, char *argv[], struct TodoListMetadata *metadata) {
    if (argc != 1) {
        printf("\"tick\" takes only one argument, %d provided\n", argc);
        print_tick_help(stdout);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(argv[0], &identifier) == UNKNOWN_ERROR) {
        printf("error: invalid argument");
        print_tick_help(stdout);
        return UNKNOWN_ERROR;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = COMPLETED;
        return EXECUTION_SUCCESS;
    }

    return TODO_ITEM_NOT_FOUND;
};

void print_tick_help(FILE *fp) {
    fprintf(fp, "Usage: todo tick ITEM_ID\n\n");
    fprintf(fp, "Description: %s\n", tick_subcommand.description);
    fprintf(fp, "Example: todo tick 2\n");
}

const struct SubCommand tick_subcommand = {
    .name = "tick",
    .description = "tick an item as done",
    .run = run_tick,
    .help_printer = print_tick_help
};

