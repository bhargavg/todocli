#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-tick.h"
#include "common.h"
#include "item.h"
#include "cmd-init.h"

void print_tick_help(FILE *);

int run_tick(int argc, char *argv[]) {
    if (argc != 1) {
        printf("\"tick\" takes only one argument, %d provided\n", argc);
        print_tick_help(stdout);
        return INVALID_ARGUMENTS;
    }

    int index = 0;
    if (sanitized_index_arg_value(argv[0], &index) == INVALID_ARGUMENTS) {
        printf("error: invalid argument");
        print_tick_help(stdout);
        return INVALID_ARGUMENTS;
    }

    char *file_path = todo_file_path();
    FILE *fp = NULL;
    int ret = EXECUTION_SUCCESS;

    if ((ret = open_todo_file_for_editing(file_path, &fp)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

    if ((ret = update_item_stats_at_index(fp, index, COMPLETED)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

bailout:
    free(file_path);
    fclose(fp);

    return ret;
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

