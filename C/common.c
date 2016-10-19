#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "common.h"
#include "cmd-init.h"

char *todo_dir_path() {
    const char *homedir = getenv("HOME");
    if (!homedir) {
        printf("HOME environment variable not set");
        exit(ENV_HOME_NOT_SET);
    }

    char *path = (char *)malloc(strlen(homedir) + 1);
    strcpy(path, homedir);
    strcat(path, "/.todo/");

    return path;
}

char *todo_file_path() {

    char *directory = todo_dir_path();
    //FIXME: path separators issue, not cross platform
    char *file = "todo.txt";
    int length = strlen(directory) + strlen(file);
    char *file_path = (char *)malloc(length + 1);

    //TODO: check if snprintf can be used here
    strcpy(file_path, directory);
    strcat(file_path, file);

    free(directory);

    return file_path;
}

int seek_till_item_index(FILE *fp, int index) {
    int i = 1;
    int ret = EXECUTION_SUCCESS;
    while (!feof(fp) 
            && i < index
            && ((ret = seek_till_next_item(fp)) == EXECUTION_SUCCESS)) {
        i++;
    }

    return ret;
}

int seek_till_next_item(FILE *fp) {
    enum ItemStatus status = NOT_COMPLETED;
    size_t string_length = 0;

    if (fread(&status, sizeof(int), 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fread(&string_length, sizeof(size_t), 1, fp) != 1) {
        return UNKNOWN_ERROR;
    }

    if (fseek(fp, string_length, SEEK_CUR) != 0) {
        return UNKNOWN_ERROR;
    }

    return EXECUTION_SUCCESS;
}

int update_item_status(FILE *fp, enum ItemStatus status) {
    int bytes_written = fwrite(&status, sizeof(int), 1, fp);
    return (bytes_written == 1) ? EXECUTION_SUCCESS : UNKNOWN_ERROR;
}

int update_item_stats_at_index(FILE *fp, int index, enum ItemStatus status) {
    int ret = EXECUTION_SUCCESS;

    if ((ret = seek_till_item_index(fp, index)) != EXECUTION_SUCCESS) {
        printf("error: Unknown error, couldn't retrive todo item for editing");
        goto bailout;
    }

    if ((ret = update_item_status(fp, status)) != EXECUTION_SUCCESS) {
        printf("error: Unknown error, couldn't modify the todo item");
        goto bailout;
    }

bailout:
    return ret;
}

int open_todo_file_for_editing(char *file_path, FILE **fp){
    FILE *file_p = fopen(file_path, "rb+");

    if (file_p == NULL) {
        if (errno == ENOENT && is_initialized()){
            printf("It looks like you don't have any todo items. Add one by using \"todo add TODO_TEXT\"\n");
            return EXECUTION_SUCCESS;
        } else {
            printf("error: %s, failed to open the todo.txt file\n", strerror(errno));
            return errno;
        }
    }

    *fp = file_p;

    return EXECUTION_SUCCESS;
}

int sanitized_index_arg_value(char *arg, int *index) {
    long value = strtol(arg, NULL, 10);

   if (value > INT_MAX || value < 0) {
       return INVALID_ARGUMENTS;
   }

   *index = value;
   return EXECUTION_SUCCESS;
}

// From: http://stackoverflow.com/a/4770992
bool has_prefix(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int process_arguments(int argc, char *argv[], int allowed_args_count, struct Argument allowed_args[], void *options_bag, struct Argument values[], int *values_count, char *invalid_args[], int *invalid_args_count) {

    *values_count = 0;
    *invalid_args_count = 0;

    //FIXME: O(n^2), optimize this
    for (int i = 0; i < argc; i++) {
        char *input_arg_string = argv[i];
        bool arg_found = false;

        if (!has_prefix(input_arg_string, "-")) {
            // this is value
            struct Argument value = { .long_name = NULL, 
                                      .short_name = NULL, 
                                      .type = VALUE, 
                                      .value_processor = NULL };

            values[(*values_count)++] = value;
            continue;
        }

        for (int j = 0; j < allowed_args_count; j++) {
            struct Argument arg = allowed_args[j];

            if (strcmp(input_arg_string, arg.long_name) == 0
                || strcmp(input_arg_string, arg.short_name) == 0) {

                arg_found = true;

                char *value = NULL;

                if (arg.type != FLAG) {
                    // read the argument value
                    i++;
                    value = (i < argc) ? argv[i] : NULL;
                }

                if (arg.value_processor(value, options_bag) != EXECUTION_SUCCESS) {
                    invalid_args[(*invalid_args_count)++] = input_arg_string;
                }

                break;
            }
        }

        if (!arg_found) {
            invalid_args[(*invalid_args_count)++] = input_arg_string;
        }
    }

    return (*invalid_args_count > 0) ? INVALID_ARGUMENTS : EXECUTION_SUCCESS;
}

