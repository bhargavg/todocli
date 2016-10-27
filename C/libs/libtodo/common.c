#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int write_metadata(struct TodoListMetadata *metadata, FILE *fp) {
    unsigned long int version = metadata->version;
    unsigned long int items_count = available_items_count(metadata);

    if (fwrite(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }
    if (fwrite(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}

int read_metadata(struct TodoListMetadata **metadata, FILE *fp) {
    unsigned long int version = 0;
    unsigned long int items_count = 0;

    if (fread(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    struct TodoListMetadata *data = malloc(sizeof(struct TodoListMetadata));
    data->version = version;
    data->items_count = items_count;
    data->items = NULL;

    *metadata = data;

    return EXECUTION_SUCCESS;
}

bool is_file_empty(FILE *fp) {
    if (getc(fp) == EOF) {
        return true;
    }

    rewind(fp);
    return false;
}

void free_todo_metadata(struct TodoListMetadata *data) {
    for (unsigned long int i = 0; i < data->items_count; i++) {
        free(data->items[i]);
    }
    free(data->items);
    free(data);
}

unsigned long int available_items_count(struct TodoListMetadata *metadata) {
    unsigned long int count = 0;
    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];
        if (item->status != REMOVED) {
            count++;
        }
    }
    return count;
}
