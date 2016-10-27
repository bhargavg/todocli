#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-untick.h"
#include "../common.h"
#include "cmd-init.h"
#include "libtodo.h"


void print_untick_help(FILE *fp);

int run_untick(int argc, char *argv[], struct TodoListMetadata *metadata) {
    if (argc != 1) {
        printf("\"untick\" takes only one argument, %d provided\n", argc);
        print_untick_help(stdout);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(argv[0], &identifier) == UNKNOWN_ERROR) {
        printf("error: invalid argument");
        print_untick_help(stdout);
        return UNKNOWN_ERROR;
    }

    struct TodoItem *item = item_with_identifier(identifier, metadata);
    if (item) {
        item->status = NOT_COMPLETED;
        return EXECUTION_SUCCESS;
    }

    return TODO_ITEM_NOT_FOUND;
};

void print_untick_help(FILE *fp) {
    fprintf(fp, "Usage: todo untick ITEM_ID\n\n");
    fprintf(fp, "Description: %s\n", untick_subcommand.description);
    fprintf(fp, "Example: todo untick 2\n");
}

const struct SubCommand untick_subcommand = {
    .name = "untick",
    .description = "untick an item as done",
    .run = run_untick,
    .help_printer = print_untick_help
};

