#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-add.h"
#include "../common.h"
#include "../argument-parser.h"
#include "libtodo.h"

int run_add(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count != 1) {
        //FIXME: handle this in is_add_ags_valid
        // printf("\"add\" takes only one argument, %d provided\n", options->values_count);
        return UNKNOWN_ERROR;
    }

    char *text = options->values[0];
    // FIXME: Make the identifier generation transparent
    struct TodoItem *item = create_todo_item(0, NOT_COMPLETED, text);

    return add_item(item, metadata);
}

bool is_add_args_valid(char **message) {
    return true;
}

const struct SubCommand add_subcommand = {
    .name = "add",
    .run = run_add,
    .help_text = "Add a given item to todo list\n\n"
                 "Usage: todo add TODO_TEXT\n\n"
                 "Options: \n"
                 "  -d, --dir\n"
                 "      Path to directory in which todo is initialized\n"
};

