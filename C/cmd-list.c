#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "cmd-init.h"
#include "common.h"
#include "item.h"

int run_list(int argc, char *argv[]) {

    char *file = todo_file_path();
    FILE *fp = fopen(file, "rb");

    if (fp == NULL) {
        if (errno == ENOENT && is_initialized()){
            printf("It looks like you don't have any todo items. Add one by using \"todo add TODO_TEXT\"\n");
            return EXECUTION_SUCCESS;
        } else {
            printf("error: %s, failed to open the todo.txt file\n", strerror(errno));
            return errno;
        }
    }

    struct TodoItem *item = (struct TodoItem *)malloc(sizeof(struct TodoItem));
    int i = 1;
    while(deserialize_item_from_stream(item, fp)) {
        printf("%d: %s\n", i, item->text);
        free(item);

        item = (struct TodoItem *)malloc(sizeof(struct TodoItem));
        i++;
    }

    free(item);

    free(file);
    fclose(fp);

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

