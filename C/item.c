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


bool deserialize_item_from_stream(struct TodoItem *item, FILE *fp) {
    size_t string_length;

    fread(&((*item).status), sizeof(int), 1, fp);
    if (feof(fp)) {
        return false;
    }

    fread(&string_length, sizeof(size_t), 1, fp);
    if (feof(fp)) {
        return false;
    }

    item->text = (char *)malloc((string_length + 1) * sizeof(char));
    fread(item->text, sizeof(char), string_length, fp);
    if (feof(fp)) {
        return false;
    }

    //FIXME: change this to proper pointer arithmetic for better readability
    (item->text)[string_length] = '\0';

    return true;
}
