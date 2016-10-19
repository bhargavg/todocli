#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-untick.h"
#include "common.h"
#include "item.h"
#include "cmd-init.h"


void print_untick_help(FILE *fp);

int run_untick(int argc, char *argv[]) {
    if (argc != 1) {
        printf("\"untick\" takes only one argument, %d provided\n", argc);
        print_untick_help(stdout);
        return INVALID_ARGUMENTS;
    }

    int index = 0;
    if (sanitized_index_arg_value(argv[0], &index) == INVALID_ARGUMENTS) {
        printf("error: invalid argument");
        print_untick_help(stdout);
        return INVALID_ARGUMENTS;
    }

    char *file_path = todo_file_path();
    FILE *fp = NULL;
    int ret = EXECUTION_SUCCESS;

    if ((ret = open_todo_file_for_editing(file_path, &fp)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

    if ((ret = update_item_stats_at_index(fp, index, NOT_COMPLETED)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

bailout:
    fflush(fp);
    free(file_path);
    fclose(fp);

    return ret;
};

void print_untick_help(FILE *fp) {
    fprintf(fp, "Usage: todo untick ITEM_ID\n\n");
    fprintf(fp, "Description: %s\n", untick_subcommand.description);
    fprintf(fp, "Example: todo untick 2\n");
}

const struct SubCommand untick_subcommand = {
    .name = "untick",
    .description = "untick an item as done",
    .run = run_untick
};

