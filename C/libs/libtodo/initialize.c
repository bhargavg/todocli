#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "libtodo.h"

int read_metadata(FILE *fp, struct TodoListMetadata *metadata);
bool is_file_empty(FILE *fp);
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

    struct TodoListMetadata *data = malloc(sizeof(struct TodoListMetadata));
    data->fp = fp;

    if (read_metadata(fp, data) != EXECUTION_SUCCESS) {
        printf("error: unknown error occured\n");
        fclose(fp);
        free(metadata);
        free(file_path);
        return UNKNOWN_ERROR;
    }

    struct TodoItem *items = malloc(sizeof(struct TodoListMetadata) * data->items_count);
    for (int i = 0; i < data->items_count; i++) {
        if (read_item_from_stream(&items[i], fp) != EXECUTION_SUCCESS) {
            printf("error: unknown error occured\n");
        }
    }

    rewind(fp);
    *metadata = data;

    free(file_path);

    return ret;
}

bool is_file_empty(FILE *fp) {
    if (getc(fp) == EOF) {
        return true;
    }

    rewind(fp);
    return false;
}

int write_default_metadata(FILE *fp) {
    unsigned long int version = metadata_current_version;
    unsigned long int items_count = 0;

    if (fwrite(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }
    if (fwrite(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}

int read_metadata(FILE *fp, struct TodoListMetadata *metadata) {
    unsigned long int version = 0;
    unsigned long int items_count = 0;

    rewind(fp);

    if (fread(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    metadata->version = version;
    metadata->items_count = items_count;

    return EXECUTION_SUCCESS;
}
