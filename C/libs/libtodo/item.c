#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "libtodo.h"

int write_item_to_stream(struct TodoItem item, FILE *fp) {
    size_t text_length = (item.text == NULL) ? 0 : strlen(item.text);

    if (fwrite(&item.identifier, metadata_item_id_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fwrite(&item.status, metadata_item_status_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fwrite(&text_length, metadata_item_text_length_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fwrite(item.text, sizeof(char), text_length, fp) != text_length) {
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}


int read_item_from_stream(struct TodoItem *todo_item, FILE *fp) {
    unsigned long int identifier = 0;
    char status = 0;
    size_t string_length = 0;
    char *text = NULL;

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

    return EXECUTION_SUCCESS;
}

int update_status(struct TodoItem item, FILE *fp) {
    if (seek_to_item_with_identifier(item.identifier, fp) != EXECUTION_SUCCESS) {
        return UNKNOWN_ERROR;
    }

    if (fwrite(&(item.status), metadata_item_status_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    } 

    return EXECUTION_SUCCESS;
}

int item_with_identifier(unsigned long int identifier, struct TodoItem *item, FILE *fp) {
    int ret = EXECUTION_SUCCESS;
    if ((ret = seek_to_item_with_identifier(identifier, fp)) != EXECUTION_SUCCESS) {
        return ret;
    }

    return read_item_from_stream(item, fp);
}

int seek_to_item_with_identifier(unsigned long int identifier, FILE *fp) {
    // move to first item;
    if (seek_to_first_item(fp) != EXECUTION_SUCCESS){
        return UNKNOWN_ERROR;
    }

    unsigned long int item_id = 0;
    bool found = false;
    int ret = EXECUTION_SUCCESS;

    while (true) {
        if (fread(&item_id, metadata_item_id_byte_count, 1, fp) != 1) {
            ret = UNKNOWN_ERROR;
            break;
        }
        if (identifier == item_id) {
            // unread the identifier
            fseek(fp, -metadata_item_id_byte_count, SEEK_CUR);
            ret = EXECUTION_SUCCESS;
            break;
        }

        if (fseek(fp, (metadata_item_id_byte_count + metadata_item_status_byte_count), SEEK_CUR) != 0) {
            ret = UNKNOWN_ERROR;
            break;
        }

        size_t text_size = 0;
        if (fread(&text_size, metadata_item_text_length_byte_count, 1, fp) != 1) {
            ret = UNKNOWN_ERROR;
            break;
        }

        if (fseek(fp, text_size, SEEK_CUR) != 0) {
            ret = UNKNOWN_ERROR;
            break;
        }
    }

    return ret;
}

int seek_to_first_item(FILE *fp) {
    return (fseek(fp, metadata_version_string_byte_count + metadata_items_byte_count, SEEK_SET) == 0) ? EXECUTION_SUCCESS : UNKNOWN_ERROR;
}

bool is_item_completed(struct TodoItem item) {
    return (item.status == COMPLETED);
}

bool is_item_removed(struct TodoItem item) {
    return (item.status == REMOVED);
}
