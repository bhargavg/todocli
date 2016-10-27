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
#include "helpers.h"

extern char *dir_path;
extern char *file_path;

MU_TEST(test_initialization_from_scratch) {
    remove_file(file_path);
    remove_directory(dir_path);

    struct TodoListMetadata *data;
    mu_check(!is_initialized(dir_path));
    mu_check(initialize(dir_path) == EXECUTION_SUCCESS);
    mu_check(load_metadata(dir_path, &data) == EXECUTION_SUCCESS);
    mu_check(data->version == metadata_current_version);
    mu_check(data->items_count == 0);

    free(data);

    remove_file(file_path);
    remove_directory(dir_path);
}

MU_TEST(test_initialization_after_already_initialized) {
    remove_file(file_path);
    remove_directory(dir_path);

    create_directory(dir_path);
    create_todo_file_with_metadata(file_path, 5, 0);

    struct TodoListMetadata *data;

    mu_check(is_initialized(dir_path));
    mu_check(initialize(dir_path) == ALREADY_INITIALIZED);
    mu_check(load_metadata(dir_path, &data) == EXECUTION_SUCCESS);
    mu_check(data->version == 5);
    mu_check(data->items_count == 0);

    free(data);

    remove_file(file_path);
    remove_directory(dir_path);
}

MU_TEST_SUITE(initialization_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_initialization_from_scratch);
    MU_RUN_TEST(test_initialization_after_already_initialized);
}

#endif
