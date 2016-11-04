#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

void print_rm_help(FILE *fp);

int run_rm(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count != 1) {
        printf("\"rm\" takes only one argument, %d provided\n", options->values_count);
        print_rm_help(stdout);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(options->values[0], &identifier) == UNKNOWN_ERROR) {
        printf("error: invalid argument");
        print_rm_help(stdout);
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
    .description = "Remove todo items",
    .run = run_rm,
    .help_printer = print_rm_help
};

void print_rm_help(FILE *fp) {
    fprintf(fp, "Usage: todo rm ITEM_ID\n");
    fprintf(fp, "       todo rm [--all | -a]\n");
    fprintf(fp, "       todo rm [--finished | -f]\n\n");
    fprintf(fp, "Description: %s\n", rm_subcommand.description);
    fprintf(fp, "Example: todo rm 2\n");
}
