#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"

void die_if_error(int status) {
    if (status != EXECUTION_SUCCESS) {
        printf("error: unknown error");
        exit(status);
    }
}

int main(int argc, char *argv[]) {

    struct TodoListMetadata *metadata;

    die_if_error(initialize("/tmp/ctodo/", &metadata));

    printf("Version: %lu, items: %lu\n", metadata->version, metadata->items_count);

    printf("Listing items:\n");
    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];
        printf("%s\n", item->text);
    }

    if (metadata->items_count == 0) {
        struct TodoItem *item = NULL;

        item = create_todo_item(1, COMPLETED, "Oolala");
        add_item(item, metadata);

        item = create_todo_item(2, NOT_COMPLETED, "Oh yeah");
        add_item(item, metadata);

        item = create_todo_item(3, REMOVED, "Ding Dong");
        add_item(item, metadata);

        FILE *fp = fopen("/tmp/ctodo/todo.bin", "wb");
        if (fp == NULL) {
            printf("error: can't open todo.bin file for writing");
            exit(1);
        }
        die_if_error(write_to_stream(metadata, fp));
        fclose(fp);
    }

    free_todo_metadata(metadata);

    return 0;
}

