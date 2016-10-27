#ifndef _ITEM_UPDATE_TESTS__
#define _ITEM_UPDATE_TESTS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../libtodo.h"
#include "libs/minunit.h"

MU_TEST(test_update_status) {
    remove_file(file_path);
    remove_directory(dir_path);

    struct TodoListMetadata *metadata;
    mu_check(!is_initialized(dir_path));
    mu_check(initialize(dir_path) == EXECUTION_SUCCESS);
    mu_check(load_metadata(dir_path, &metadata) == EXECUTION_SUCCESS);

    struct TodoItem *item = create_todo_item(1, NOT_COMPLETED, "Oolala");
    mu_check(add_item(item, metadata) == EXECUTION_SUCCESS);

    FILE *fp = fopen(file_path, "wb");
    mu_check(fp != NULL);

    mu_check(write_to_stream(metadata, fp) == EXECUTION_SUCCESS);
    fclose(fp);
    free_todo_metadata(metadata);

    mu_check(load_metadata(dir_path, &metadata) == EXECUTION_SUCCESS);
    mu_check(metadata->items_count == 1);

    metadata->items[0]->status = COMPLETED;

    fp = fopen(file_path, "wb");
    mu_check(fp != NULL);
    mu_check(write_to_stream(metadata, fp) == EXECUTION_SUCCESS);

    fclose(fp);
    free_todo_metadata(metadata);

    mu_check(load_metadata(dir_path, &metadata) == EXECUTION_SUCCESS);
    mu_check(metadata->items_count == 1);
    mu_check(metadata->items[0]->status == COMPLETED);

    free_todo_metadata(metadata);
    remove_file(file_path);
    remove_directory(dir_path);
}

MU_TEST_SUITE(item_status_update_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_update_status);
}
#endif
