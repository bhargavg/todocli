#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "libtodo.h"
#include "common.h"

int write_default_metadata(FILE *fp);

bool is_initialized(char *dir_path) {
    struct stat sb;
    return (stat(dir_path, &sb) == 0 && S_ISDIR(sb.st_mode));
}

int initialize(char *dir_path) {
    if (mkdir(dir_path, 0755) != 0) {
        if (errno == EEXIST) {
            return ALREADY_INITIALIZED;
        }
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}

int load_metadata(char *dir_path, struct TodoListMetadata **metadata) {
    char *file_path = malloc(strlen(dir_path) + strlen(todo_file_name) + 1);
    strcpy(file_path, dir_path);
    strcat(file_path, todo_file_name);

    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL) {
        // might be file doesn't exist, try to open in write mode
        fp = fopen(file_path, "wb");
        if (fp == NULL) {
            free(file_path);
            return is_initialized(dir_path) ? TODO_FILE_READ_ERROR : NOT_INITIALIZED;
        }

        if(write_default_metadata(fp) != EXECUTION_SUCCESS) {
            free(file_path);
            fclose(fp);
            return TODO_FILE_WRITE_ERROR;
        }

        fclose(fp);

        fp = fopen(file_path, "rb");

        if (fp == NULL) {
            free(file_path);
            return TODO_FILE_READ_ERROR;
        }
    }

    struct TodoListMetadata *data = NULL;

    if (read_metadata(&data, fp) != EXECUTION_SUCCESS) {
        fclose(fp);
        free_todo_metadata(data);
        free(file_path);
        return TODO_FILE_READ_ERROR;
    }

    struct TodoItem **items = malloc(sizeof(struct TodoListMetadata *) * data->items_count);
    for (int i = 0; i < data->items_count; i++) {
        struct TodoItem *item = NULL;
        if (read_item_from_stream(&item, fp) != EXECUTION_SUCCESS
            || item == NULL) {
            fclose(fp);
            free_todo_metadata(data);
            free(file_path);
            free_todo_item(item);
            return TODO_FILE_READ_ERROR;
        }

        items[i] = item;
    }

    fclose(fp);

    data->items = items;
    *metadata = data;

    return EXECUTION_SUCCESS;
}

int write_default_metadata(FILE *fp) {
    struct TodoListMetadata *metadata = malloc(sizeof(struct TodoListMetadata));

    metadata->version = metadata_current_version;
    metadata->items_count = 0;
    metadata->items = NULL;

    int ret = write_metadata(metadata, fp);

    free_todo_metadata(metadata);
    
    return ret;
}
