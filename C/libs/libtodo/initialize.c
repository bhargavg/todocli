#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "libtodo.h"
#include "common.h"

int write_default_metadata(FILE *fp);

int initialize(char *dir_path, struct TodoListMetadata **metadata) {
    int ret = EXECUTION_SUCCESS;

    char *file_path = malloc(strlen(dir_path) + strlen(todo_file_name) + 1);
    strcpy(file_path, dir_path);
    strcat(file_path, todo_file_name);

    if (mkdir(dir_path, 0755) != 0
            && errno != EEXIST) {
        printf("error: %s, failed to create the todo directory: %s\n", strerror(errno), dir_path);
        free(file_path);
        return UNKNOWN_ERROR;
    }

    FILE *fp = fopen(file_path, "rb+");
    if (fp == NULL) {
        // try to open in write mode
        fp = fopen(file_path, "wb");
        if (fp == NULL) {
            printf("error: %s, failed to create the todo file: %s\n", strerror(errno), file_path);
            free(file_path);
            return UNKNOWN_ERROR;
        }

        if(write_default_metadata(fp) != EXECUTION_SUCCESS) {
            printf("error: unable to write default metadata to todo file: %s\n", file_path);
            free(file_path);
            fclose(fp);
            return UNKNOWN_ERROR;
        }

        fclose(fp);

        fp = fopen(file_path, "rb+");

        if (fp == NULL) {
            printf("error: %s, failed to create the todo file: %s\n", strerror(errno), file_path);
            free(file_path);
            return UNKNOWN_ERROR;
        }
    }


    if (is_file_empty(fp)) {
        printf("File is empty\n");
        printf("Writing defaults... ");
        if (write_default_metadata(fp) != EXECUTION_SUCCESS) {
            printf("error: unable to write default metadata to todo file: %s\n", file_path);
            free(file_path);
            fclose(fp);
            return UNKNOWN_ERROR;
        }
        printf("done\n");
    }

    struct TodoListMetadata *data = NULL;

    if (read_metadata(&data, fp) != EXECUTION_SUCCESS) {
        printf("error: unknown error occured\n");
        fclose(fp);
        free_todo_metadata(data);
        free(file_path);
        return UNKNOWN_ERROR;
    }

    struct TodoItem **items = malloc(sizeof(struct TodoListMetadata *) * data->items_count);
    for (int i = 0; i < data->items_count; i++) {
        struct TodoItem *item = NULL;
        if (read_item_from_stream(&item, fp) != EXECUTION_SUCCESS
            || item == NULL) {
            printf("error: unknown error occured\n");
            fclose(fp);
            free_todo_metadata(data);
            free(file_path);
            free_todo_item(item);
            return UNKNOWN_ERROR;
        }

        items[i] = item;
    }

    fclose(fp);

    data->items = items;
    *metadata = data;

    return ret;
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
