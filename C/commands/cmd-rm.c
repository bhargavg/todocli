#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../common.h"
#include "libtodo.h"

void print_rm_help(FILE *fp);

struct RemovingOptions {
    int item_number;
    bool should_remove_at_index;
    bool should_remove_all;
    bool should_remove_finished;
};

int process_arg_remove_all(char *arg, void *options_bag) {
    struct RemovingOptions *options = options_bag;
    options->should_remove_all = true;
    return EXECUTION_SUCCESS;
}

int process_arg_remove_finished(char *arg, void *options_bag) {
    struct RemovingOptions *options = options_bag;
    options->should_remove_finished = true;
    return EXECUTION_SUCCESS;
}

int process_arg_item_id(char *arg, void *options_bag) {
    struct RemovingOptions *options = options_bag;

    unsigned long int value = 0;
    int ret = EXECUTION_SUCCESS;

    if ((ret = sanitized_index_arg_value(arg, &value)) == EXECUTION_SUCCESS) {
        options->item_number = value;
        options->should_remove_at_index = true;
    }

    return ret;
}

int run_rm(int argc, char *argv[], struct TodoListMetadata *metadata) {
    /*

    struct RemovingOptions options = {0};
    struct Argument allowed_args[] = {
        { .long_name = "--all", .short_name = "-a", .type = FLAG, .value_processor = process_arg_remove_all},
        { .long_name = "--finished", .short_name = "-f", .type = FLAG, .value_processor = process_arg_remove_finished},
        { .long_name = NULL, .short_name = NULL, .type = VALUE, .value_processor = process_arg_item_id}
    };

    char *invalid_argument = NULL;

    char **invalid_arguments = (char **)calloc(argc, sizeof(char *));
    struct Argument *values  = (char **)calloc(argc, sizeof(struct Argument));
    char **arguments = (char **)calloc(argc, sizeof(char *));

    if (process_arguments(argc, argv, 1, allowed_args, &options, &invalid_argument)) {
    }

    if (process_arguments(argc, argv, 1, allowed_args, &options, &invalid_argument) != EXECUTION_SUCCESS) {
        printf("error: invalid argument: %s\n", invalid_argument);
        return INVALID_ARGUMENTS;
    }

    char *file = todo_file_path();
    FILE *fp = fopen(file, "a");
    free(file);

    int ret = EXECUTION_SUCCESS;

    if ((ret = open_todo_file_for_editing(file, &fp)) != EXECUTION_SUCCESS) {
        goto bailout;
    }

    if (options.should_remove_at_index 
        && !options.should_remove_finished 
        && !options.should_remove_all) {

        if ((ret = update_item_stats_at_index(fp, options.item_number, REMOVED)) != EXECUTION_SUCCESS) {
            goto bailout;
        }

    } if (!options.should_remove_at_index 
        && options.should_remove_finished 
        && !options.should_remove_all) {
    } if (!options.should_remove_at_index 
        && !options.should_remove_finished 
        && options.should_remove_all) {
    } else {
        printf("error: multiple arguments provided");
        return INVALID_ARGUMENTS;
    }

bailout:
    fclose(fp);

    return ret;
*/

    if (argc != 1) {
        printf("\"rm\" takes only one argument, %d provided\n", argc);
        print_rm_help(stdout);
        return UNKNOWN_ERROR;
    }

    unsigned long int identifier = 0;
    if (sanitized_index_arg_value(argv[0], &identifier) == UNKNOWN_ERROR) {
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
