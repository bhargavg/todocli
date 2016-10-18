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
    free(file);

    if (fp == NULL) {
        if (errno == ENOENT && is_initialized()){
            printf("It looks like you don't have any todo items. Add one by using \"todo add TODO_TEXT\"\n");
            return EXECUTION_SUCCESS;
        } else {
            printf("error: %s, failed to open the todo.txt file\n", strerror(errno));
            return errno;
        }
    }

    int i = 1;
    struct TodoItem *item = (struct TodoItem *)malloc(sizeof(struct TodoItem));

    while(deserialize_item_from_stream(item, fp) == EXECUTION_SUCCESS) {
        char *status = (item->status == COMPLETED) ? "✔" : " ";
        printf("%s %d: %s\n", status, i, item->text);
        i++;
    }

    free(item);
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

