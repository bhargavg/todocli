#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "argument-parser.h"
#include "common.h"

#define MAX_VALUES_COUNT 100

extern const char *todo_file_name;

int set_base_directory_option(char *value, struct Options *options) {
    options->dir_path = strdup(value);
    options->file_path = malloc(strlen(value) + strlen("/todo.bin") + 1);
    strcpy(options->file_path, value);
    strcat(options->file_path, todo_file_name);
    return 0;
}

int set_all_option(char *value, struct Options *options) {
    options->all = true;
    return 0;
}

int set_completed_option(char *value, struct Options *options) {
    options->completed = true;
    return 0;
}

int set_pending_option(char *value, struct Options *options) {
    options->pending = true;
    return 0;
}

int set_summary_option(char *value, struct Options *options) {
    options->summary = true;
    return 0;
}

struct Options *options_new() {
    struct Options *options = malloc(sizeof(struct Options));
    options->dir_path = get_default_todo_directory();

    options->file_path = malloc(sizeof(char) * (sizeof(options->dir_path) + sizeof(todo_file_name) + 1));
    strcpy(options->file_path, options->dir_path);
    strcat(options->file_path, todo_file_name);

    options->all = false;
    options->pending = false;
    options->completed = false;
    options->values = malloc(sizeof(char *) * MAX_VALUES_COUNT);
    options->values_count = 0;

    return options;
}

void free_options(struct Options *options) {
    free(options->dir_path);
    free(options->file_path);
    for (int i = 0; i < options->values_count; i++) {
        free(options->values[i]);
    }
    free(options->values);
    free(options);
}

void process_args(int argc, char *argv[], struct Argument args[], int args_count, struct Options *options) {
    for (int i = 0; i < argc; i++) {
        char *arg_key = argv[i];

        if (has_prefix(arg_key, "--") || has_prefix(arg_key, "-")) {
            for (int j = 0; j < args_count; j++) {
                struct Argument arg = args[j];

                if (strcmp(arg_key, arg.long_name) == 0
                    || strcmp(arg_key, arg.short_name) == 0) {

                    if (arg.is_flag) {
                        arg.parser(NULL, options);
                    } else {
                        i++;
                        if (i < argc) {
                            char *arg_value = argv[i];
                            arg.parser(arg_value, options);
                        } else {
                            printf("Value not found for %s", arg_key);
                        }
                    }

                    continue;
                }
            }
        } else {
            options->values[options->values_count++] = strdup(arg_key);
        }
    }
}

void print_options(struct Options options) {
    printf("Options: \n");
    printf("   dir_path: %s\n", options.dir_path);
    printf("  file_path: %s\n", options.file_path);
    printf("        all: %s\n", options.all ? "✓" : "✘");
    printf("  completed: %s\n", options.completed ? "✓" : "✘");
    printf("    pending: %s\n", options.pending ? "✓" : "✘");
    printf("    values: ");

    for (int i = 0; i < options.values_count; i++) {
        printf("%s%s", options.values[i], (i >= options.values_count - 1) ? "" : ", ");
    }

    printf("\n");
}

