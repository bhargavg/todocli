#ifndef _ITEM_REMOVAL_TESTS__
#define _ITEM_REMOVAL_TESTS__

MU_TEST(test_item_removal) {
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


    // now change the status to removed and flush to disk
    metadata->items[0]->status = REMOVED;

    fp = fopen(file_path, "wb");
    mu_check(fp != NULL);
    mu_check(write_to_stream(metadata, fp) == EXECUTION_SUCCESS);
    fclose(fp);


    // load the items back and check the count
    mu_check(load_metadata(dir_path, &metadata) == EXECUTION_SUCCESS);
    mu_check(metadata->items_count == 0);

    free_todo_metadata(metadata);

    remove_file(file_path);
    remove_directory(dir_path);
}

MU_TEST_SUITE(item_removal_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_item_removal);
}

#endif
