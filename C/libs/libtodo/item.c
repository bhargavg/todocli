#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"
#include "common.h"

int write_to_stream(struct TodoListMetadata *metadata, FILE *fp) {
    if (write_metadata(metadata, fp) != EXECUTION_SUCCESS) {
        return UNKNOWN_ERROR;
    }

    unsigned long int items_count = available_items_count(metadata);
    for (unsigned long int i = 0; i < metadata->items_count; i++) {
        struct TodoItem *item = metadata->items[i];

        if (item->status == REMOVED) {
            continue;
        }

        char *text = "";
        size_t text_length = 0;
        if (item->text != NULL) {
            text = item->text;
            text_length = strlen(text);
        }

        if (fwrite(&(item->identifier), metadata_item_id_byte_count, 1, fp) != 1) {
            return UNKNOWN_ERROR;
        }

        if (fwrite(&(item->status), metadata_item_status_byte_count, 1, fp) != 1) {
            return UNKNOWN_ERROR;
        }

        if (fwrite(&text_length, metadata_item_text_length_byte_count, 1, fp) != 1) {
            return UNKNOWN_ERROR;
        }

        if (fwrite(text, sizeof(char), text_length, fp) != text_length) {
            return UNKNOWN_ERROR;
        }
    }

    return EXECUTION_SUCCESS;
}


int read_item_from_stream(struct TodoItem **item, FILE *fp) {
    unsigned long int identifier = 0;
    char status = 0;
    size_t string_length = 0;
    char *text = NULL;

    struct TodoItem *todo_item = malloc(sizeof(struct TodoItem));

    if (fread(&identifier, metadata_item_id_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&status, metadata_item_status_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&string_length, metadata_item_text_length_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    text = (char *)malloc((string_length + 1) * sizeof(char));
    if (fread(text, sizeof(char), string_length, fp) != string_length) {
        return UNKNOWN_ERROR;
    }

    text[string_length] = '\0';

    todo_item->identifier = identifier;
    todo_item->status = status;
    todo_item->text = (text == NULL) ? "" : text;

    *item = todo_item;

    return EXECUTION_SUCCESS;
}

struct TodoItem *create_todo_item(unsigned long int identifier, enum ItemStatus status, char *text) {
    struct TodoItem *item = malloc(sizeof(struct TodoItem));
    size_t length = (text == NULL) ? 0 : strlen(text);

    item->identifier = identifier;
    item->status = status;
    item->text = malloc((length + 1) * sizeof(char));
    strncpy(item->text, text, length);

    return item;
}

void free_todo_item(struct TodoItem *item) {
    free(item->text);
    free(item);
}

int add_item(struct TodoItem *item, struct TodoListMetadata *metadata) {
    unsigned long int items_count = metadata->items_count;
    struct TodoItem **new_items = realloc(metadata->items, sizeof(struct TodoItem) * (items_count + 1));

    if (new_items == NULL) {
        return UNKNOWN_ERROR;
    }

    unsigned long int next_identifier = 1;

    if (metadata->items_count > 0) {
        struct TodoItem *last_item = metadata->items[metadata->items_count - 1];
        next_identifier = last_item->identifier + 1;
    }

    item->identifier = next_identifier;

    new_items[metadata->items_count] = item;

    metadata->items = new_items;
    metadata->items_count++;

    return EXECUTION_SUCCESS;
}

struct TodoItem *item_with_identifier(unsigned long int identifier, struct TodoListMetadata *metadata) {
    struct TodoItem *item = NULL;
    for (unsigned int i = 0; i < metadata->items_count; i++) {
        if (identifier == metadata->items[i]->identifier) {
            item = metadata->items[i];
            break;
        }
    }

    return item;
}
