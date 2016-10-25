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
    struct stat sb = {0};
    int ret = EXECUTION_SUCCESS;

    if(stat(dir_path, &sb) != 0
        || !S_ISDIR(sb.st_mode)) {
        int creation_status = mkdir(dir_path, 0755);
        if (creation_status != 0) {
            printf("error: unable to create directory - %s (%s)\n", dir_path, strerror(errno));
            return creation_status;
        }
    }

    char *file_path = malloc(strlen(dir_path) + strlen(todo_file_name) + 1);
    strcpy(file_path, dir_path);
    strcat(file_path, todo_file_name);

    FILE *fp = fopen(file_path, "rb+");

    if (fp == NULL) {
        if(errno == ENOENT) {
            // file doesn't exist, create one
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
        } else {
            printf("error: %s, failed to create the todo file: %s\n", strerror(errno), file_path);
            free(file_path);
            return UNKNOWN_ERROR;
        }
    }

    struct TodoListMetadata *data = malloc(sizeof(struct TodoListMetadata));
    data->fp = fp;

    if (is_file_empty(fp)) {
        if (write_default_metadata(fp) != EXECUTION_SUCCESS) {
            printf("error: unable to write default metadata to todo file: %s\n", file_path);
            free(file_path);
            free(data);
            fclose(fp);
            return UNKNOWN_ERROR;
        }
    }

    if (read_metadata(fp, data) != EXECUTION_SUCCESS) {
        printf("error: unknown error occured\n");
        fclose(fp);
        free(metadata);
        free(file_path);
        return UNKNOWN_ERROR;
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
    rewind(fp);
    if (fread(&(metadata->version), metadata_version_string_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&(metadata->items_count), metadata_items_byte_count, 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}
