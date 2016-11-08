#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    SET_FLAG((*options), OPTIONS_FLAG_ALL);
    return 0;
}

int set_completed_option(char *value, struct Options *options) {
    SET_FLAG((*options), OPTIONS_FLAG_COMPLETED);
    return 0;
}

int set_pending_option(char *value, struct Options *options) {
    SET_FLAG((*options), OPTIONS_FLAG_PENDING);
    return 0;
}

int set_summary_option(char *value, struct Options *options) {
    SET_FLAG((*options), OPTIONS_FLAG_SUMMARY);
    return 0;
}

int set_help_option(char *value, struct Options *options) {
    SET_FLAG((*options), OPTIONS_FLAG_HELP);
    return 0;
}

struct Options *options_new() {
    struct Options *options = malloc(sizeof(struct Options));
    options->dir_path = get_default_todo_directory();

    options->file_path = malloc(sizeof(char) * (sizeof(options->dir_path) + sizeof(todo_file_name) + 1));
    strcpy(options->file_path, options->dir_path);
    strcat(options->file_path, todo_file_name);

    CLR_ALL_FLAGS((*options));

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

void print_options(struct Options options) {
    printf("Options: \n");
    printf("   dir_path: %s\n", options.dir_path);
    printf("  file_path: %s\n", options.file_path);
    printf("        all: %s\n", IS_FLAG_SET(options, OPTIONS_FLAG_ALL) ? "✓" : "✘");
    printf("  completed: %s\n", IS_FLAG_SET(options, OPTIONS_FLAG_COMPLETED) ? "✓" : "✘");
    printf("    pending: %s\n", IS_FLAG_SET(options, OPTIONS_FLAG_PENDING) ? "✓" : "✘");
    printf("    summary: %s\n", IS_FLAG_SET(options, OPTIONS_FLAG_SUMMARY) ? "✓" : "✘");
    printf("       HELP: %s\n", IS_FLAG_SET(options, OPTIONS_FLAG_HELP) ? "✓" : "✘");
    printf("    values: ");

    for (int i = 0; i < options.values_count; i++) {
        printf("%s%s", options.values[i], (i >= options.values_count - 1) ? "" : ", ");
    }

    printf("\n");
}
