#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "common.h"
#include "item.h"

int run_list(int argc, char *argv[]) {

    char *file = todo_file_path();
    FILE *fp = fopen(file, "rb");

    if (fp == NULL) {
        printf("error: %s, failed to open the todo.txt file", strerror(errno));
        return errno;
    }

    struct TodoItem *item = (struct TodoItem *)malloc(sizeof(struct TodoItem));
    while(deserialize_item_from_stream(item, fp)) {
        printf("item: %s", item->text);
        free(item);

        item = (struct TodoItem *)malloc(sizeof(struct TodoItem));
    }

    free(item);

    free(file);
    fclose(fp);

    return EXECUTION_SUCCESS;
};

const struct SubCommand list_subcommand = {
    "list",
    run_list
};

