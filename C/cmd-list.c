#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "cmd-init.h"
#include "common.h"
#include "item.h"

void print_list_help(FILE *fp);

struct ListingOptions {
    bool show_only_pending;
};

int process_show_only_pending_value(char *arg, void *options_bag) {
    struct ListingOptions *options = options_bag;
    options->show_only_pending = true;
    return EXECUTION_SUCCESS;
}


int run_list(int argc, char *argv[]) {

    struct ListingOptions options = {0};

    struct Argument allowed_args[] = {
        {.long_name = "--pending", .short_name = "-p", .type = FLAG, .value_processor = process_show_only_pending_value}
    };


    char *invalid_argument = NULL;
    if (process_arguments(argc, argv, 1, allowed_args, &options, &invalid_argument) != EXECUTION_SUCCESS) {
        printf("error: invalid argument: %s\n", invalid_argument);
        return INVALID_ARGUMENTS;
    }


    char *file_path = todo_file_path();
    FILE *fp = NULL;
    int ret = EXECUTION_SUCCESS;

    if ((ret = open_todo_file_for_editing(file_path, &fp)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

    int i = 1;
    struct TodoItem *item = (struct TodoItem *)malloc(sizeof(struct TodoItem));

    while(deserialize_item_from_stream(item, fp) == EXECUTION_SUCCESS) {
        if (!options.show_only_pending
                || !is_item_completed(*item)) {
            char *status = is_item_completed(*item) ? "✔" : " ";
            printf("%s %d: %s\n", status, i, item->text);
        }

        i++;
    }


bailout:
    fclose(fp);
    free(file_path);

    return EXECUTION_SUCCESS;
};

const struct SubCommand list_subcommand = {
    .name = "list",
    .description = "Print all the todo items",
    .run = run_list,
    .help_printer = print_list_help
};

void print_list_help(FILE *fp) {
    fprintf(fp, "Usage: todo list\n\n");
    fprintf(fp, "Description: %s\n", list_subcommand.description);
}

