#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "cmd-init.h"
#include "common.h"
#include "item.h"

struct ListingOptions {
    bool show_only_pending;
};

int process_show_only_pending_value(char *arg, void *options_bag) {
    struct ListingOptions *options = options_bag;
    options->show_only_pending = true;
    return EXECUTION_SUCCESS;
}

int process_arguments(int argc, char *argv[], int allowed_args_count, struct Argument allowed_args[], struct ListingOptions *options, char **invalid_argument) {

    //FIXME: O(n^2), optimize this
    for (int i = 0; i < argc; i++) {
        char *input_arg_string = argv[i];
        bool arg_found = false;

        for (int j = 0; j < allowed_args_count; j++) {
            struct Argument arg = allowed_args[j];

            if (strcmp(input_arg_string, arg.long_name) == 0
                || strcmp(input_arg_string, arg.short_name) == 0) {

                arg_found = true;

                char *value = NULL;

                if (!arg.is_flag) {
                    // read the argument value
                    i++;
                    value = (i < argc) ? argv[i] : NULL;
                }

                if (arg.value_processor(value, options) != EXECUTION_SUCCESS) {
                    *invalid_argument = input_arg_string;
                }

                break;
            }
        }

        if (!arg_found) {
            *invalid_argument = input_arg_string;
        }
    }

    return *invalid_argument ? INVALID_ARGUMENTS : EXECUTION_SUCCESS;
}


int run_list(int argc, char *argv[]) {

    struct ListingOptions options = {0};

    struct Argument allowed_args[] = {
        {.long_name = "--pending", .short_name = "-p", .is_flag = true, .value_processor = process_show_only_pending_value}
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

void print_list_help(FILE *fp) {
    fprintf(fp, "Usage: todo list\n\n");
    fprintf(fp, "Description: %s\n", list_subcommand.description);
}

const struct SubCommand list_subcommand = {
    .name = "list",
    .description = "Print all the todo items",
    .run = run_list
};

