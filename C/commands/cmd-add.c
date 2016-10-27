#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-add.h"
#include "../common.h"
#include "libtodo.h"

void print_add_help(FILE *);

int run_add(int argc, char *argv[], struct TodoListMetadata *metadata) {
    if (argc != 1) {
        printf("\"add\" takes only one argument, %d provided\n", argc);
        print_add_help(stdout);
        return UNKNOWN_ERROR;
    }

    char *text = argv[0];
    // FIXME: Make the identifier generation transparent
    struct TodoItem *item = create_todo_item(0, NOT_COMPLETED, text);

    return add_item(item, metadata);
}

void print_add_help(FILE *fp) {
    fprintf(fp, "Usage: todo add TODO_TEXT\n\n");
    fprintf(fp, "Description: %s\n", add_subcommand.description);
    fprintf(fp, "Example: todo add \"Make Todo great again\"\n");
}

const struct SubCommand add_subcommand = {
    .name = "add",
    .description = "Add a given item to the todo list",
    .run = run_add,
    .help_printer = print_add_help
};

