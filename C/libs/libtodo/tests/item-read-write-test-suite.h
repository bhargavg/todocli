#ifndef _ITEM_READ_WRITE_TESTS__
#define _ITEM_READ_WRITE_TESTS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../libtodo.h"
#include "helpers.h"
#include "libs/minunit.h"

extern char *dir_path;
extern char *file_path;

MU_TEST(test_read_write_items) {
    struct TodoListMetadata *metadata;

    remove_file(file_path);
    remove_directory(dir_path);

    mu_check(initialize(dir_path, &metadata) == EXECUTION_SUCCESS);

    FILE *fp = fopen(file_path, "wb");
    mu_check(fp != NULL);

    struct TodoItem *item = NULL;
    item = create_todo_item(1, NOT_COMPLETED, "Oolala");
    mu_check(add_item(item, metadata) == EXECUTION_SUCCESS);

    item = create_todo_item(200, NOT_COMPLETED, NULL);
    mu_check(add_item(item, metadata) == EXECUTION_SUCCESS);

    item = create_todo_item(387953891, NOT_COMPLETED, "Ding Dong");
    mu_check(add_item(item, metadata) == EXECUTION_SUCCESS);

    mu_check(write_to_stream(metadata, fp) == EXECUTION_SUCCESS);

    fclose(fp);
    free_todo_metadata(metadata);
    metadata = NULL;

    mu_check(initialize(dir_path, &metadata) == EXECUTION_SUCCESS);
    mu_check(metadata->items_count == 3);
    mu_check(metadata->items[0]->identifier == 1);
    mu_check(metadata->items[0]->status == NOT_COMPLETED);
    mu_check(strcmp(metadata->items[0]->text, "Oolala") == 0);

    mu_check(metadata->items[1]->identifier == 200);
    mu_check(metadata->items[1]->status == NOT_COMPLETED);
    mu_check(strcmp(metadata->items[1]->text, "") == 0);

    mu_check(metadata->items[2]->identifier == 387953891);
    mu_check(metadata->items[2]->status == NOT_COMPLETED);
    mu_check(strcmp(metadata->items[2]->text, "Ding Dong") == 0);

    fclose(fp);

    remove_file(file_path);
    remove_directory(dir_path);
}

MU_TEST_SUITE(item_read_write_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_read_write_items);
}
#endif
