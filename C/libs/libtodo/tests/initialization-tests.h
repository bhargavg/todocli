#ifndef _INIT_TESTS__
#define _INIT_TESTS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../libtodo.h"
#include "libs/minunit.h"

void remove_todo_directory();
void remove_todo_file();
void create_todo_directory();
void create_todo_directory_with_todo_file(unsigned long int, unsigned long int);
void create_todo_file_with_metadata(unsigned long int, unsigned long int);

char *dir_path;
char *file_path;

void die(char *text) {
    perror(text);
    exit(1);
}


MU_TEST(test_initialization_from_scratch) {
    remove_todo_directory();

    struct TodoListMetadata *data;
    mu_check(initialize(dir_path, &data) == EXECUTION_SUCCESS);
    mu_check(data->version == metadata_current_version);
    mu_check(data->items_count == 0);

    fclose(data->fp);
    free(data);

    remove_todo_directory();
}

MU_TEST(test_initialization_after_already_initialized) {
    create_todo_directory_with_todo_file(5, 10);

    struct TodoListMetadata *data;

    mu_check(initialize(dir_path, &data) == EXECUTION_SUCCESS);
    mu_check(data->version == 5);
    mu_check(data->items_count == 10);

    fclose(data->fp);
    free(data);

    remove_todo_directory();
}

MU_TEST_SUITE(initialization_test_suite) {

    dir_path  = "/tmp/ctodo/";
    file_path = "/tmp/ctodo/todo.bin";

    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_initialization_from_scratch);
    MU_RUN_TEST(test_initialization_after_already_initialized);
}

void remove_todo_directory() {
    remove_todo_file();

    if (rmdir(dir_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo directory");
    }
}

void remove_todo_file() {
    if (unlink(file_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo file");
    }
}

void create_todo_directory() {
    if (mkdir(dir_path, 0755) != 0
        && errno != EEXIST) {
        die("unable to create todo directory");
    }
}

void create_todo_directory_with_todo_file(unsigned long int version, unsigned long int items_count) {
    create_todo_directory();
    create_todo_file_with_metadata(version, items_count);
}

void create_todo_file_with_metadata(unsigned long int version, unsigned long int items_count) {
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL) {
        die("unable to open todo file for writing");
    }

    if (fwrite(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        die("unable to write the contents to todo file");
    }

    if (fwrite(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        die("unable to write the contents to todo file");
    }
    fclose(fp);
}

#endif
