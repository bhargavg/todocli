#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "common.h"


void serialize_item_to_stream(const struct TodoItem item, FILE *fp) {
    size_t text_length = strlen(item.text);
    fwrite(&item.status, sizeof(int), 1, fp);
    fwrite(&text_length, sizeof(size_t), 1, fp);
    fprintf(fp, "%s", item.text);
}


int deserialize_item_from_stream(struct TodoItem *item, FILE *fp) {
    size_t string_length;

    if (fread(&((*item).status), sizeof(int), 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&string_length, sizeof(size_t), 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    item->text = (char *)malloc((string_length + 1) * sizeof(char));
    if (fread(item->text, sizeof(char), string_length, fp) != string_length) {
        return UNKNOWN_ERROR;
    }

    //FIXME: change this to proper pointer arithmetic for better readability
    (item->text)[string_length] = '\0';

    return EXECUTION_SUCCESS;
}

bool is_item_completed(struct TodoItem item) {
    return (item.status == COMPLETED);
}

bool is_item_removed(struct TodoItem item) {
    return (item.status == REMOVED);
}
