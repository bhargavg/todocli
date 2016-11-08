#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cmd-add.h"
#include "../common.h"
#include "../argument-parser.h"
#include "libtodo.h"

struct SubCommandExecResult *run_add(struct Options *options, struct TodoListMetadata *metadata) {

    struct SubCommandExecResult *result = exec_result_new();

    if (options->values_count != 1) {
        result->status = INVALID_ARGUMENT;
        asprintf(&(result->message), "\"add\" takes only one argument, %d provided\n", options->values_count);
        return result;
    }

    char *text = options->values[0];
    // FIXME: Make the identifier generation transparent
    struct TodoItem *item = create_todo_item(0, NOT_COMPLETED, text);

    result->status = add_item(item, metadata);
    result->message = strdup("Item addedd successfully!");
    return result;
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
                 "      Path to directory in which todo is initialized\n",
};

