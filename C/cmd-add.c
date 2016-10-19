#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-add.h"
#include "common.h"
#include "item.h"

void print_add_help(FILE *);

int run_add(int argc, char *argv[]) {
    if (argc != 1) {
        printf("\"add\" takes only one argument, %d provided\n", argc);
        print_add_help(stdout);
        return INVALID_ARGUMENTS;
    }

    char *file = todo_file_path();
    FILE *fp = fopen(file, "a");

    if (fp == NULL) {
        printf("error: %s, failed to write the todo item", strerror(errno));
        return errno;
    }

    struct TodoItem item = { .status = NOT_COMPLETED, .text = argv[0]};
    serialize_item_to_stream(item, fp);

    free(file);
    fclose(fp);

    printf("Added a new todo item!\n");

    return EXECUTION_SUCCESS;
};

void print_add_help(FILE *fp) {
    fprintf(fp, "Usage: todo add TODO_TEXT\n\n");
    fprintf(fp, "Description: %s\n", add_subcommand.description);
    fprintf(fp, "Example: todo add \"Make Todo great again\"\n");
}

const struct SubCommand add_subcommand = {
    .name = "add",
    .description = "Add a given item to the todo list",
    .run = run_add
};

