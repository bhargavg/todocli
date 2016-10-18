#include <stdio.h>
#include <errno.h>
#include "cmd-add.h"
#include "common.h"
#include "item.c"

int run_add(int argc, char *argv[]) {
    if (argc != 1) {
        printf("\"add\" takes only one argument, %d provided\n", argc);
        return INVALID_PARAMETERS;
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

const struct SubCommand add_subcommand = {
    .name = "add",
    .description = "Add a given item to the todo list",
    .run = run_add
};

