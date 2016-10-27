#ifndef _ITEM_READ_WRITE_TESTS__
#define _ITEM_READ_WRITE_TESTS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../libtodo.h"
#include "../item.h"
#include "libs/minunit.h"

MU_TEST(test_read_write_single_item) {
    struct TodoItem item = { .identifier = 200, .status = COMPLETED, .text = "Foo bar baz" };

    FILE *fp = fopen("/tmp/ctodo.bin", "wb");

    mu_check(fp != NULL);
    mu_check(write_item_to_stream(item, fp) == EXECUTION_SUCCESS);

    fclose(fp);

    struct TodoItem *read_item = malloc(sizeof(struct TodoItem));

    fp = fopen("/tmp/ctodo.bin", "rb");
    mu_check(fp != NULL);

    mu_check(read_item_from_stream(read_item, fp) == EXECUTION_SUCCESS);
    mu_check(read_item->identifier == 200);
    mu_check(read_item->status == COMPLETED);
    mu_check(strcmp(read_item->text, "Foo bar baz") == 0);

    free(read_item);
    unlink("/tmp/ctodo.bin");
}

MU_TEST(test_read_write_multiple_items) {
    int identifiers[] = { 200, 0, 12345 };
    enum ItemStatus statuses[] = { COMPLETED, NOT_COMPLETED, REMOVED };
    char *texts[] = { "Foo bar baz", NULL, "Hello, world!" };

    FILE *fp = fopen("/tmp/ctodo.bin", "wb");

    mu_check(fp != NULL);

    for (int i = 0; i < 3; i++) {
        struct TodoItem item = { .identifier = identifiers[i], .status = statuses[i], .text = texts[i]};
        mu_check(write_item_to_stream(item, fp) == EXECUTION_SUCCESS);
    }


    fclose(fp);


    fp = fopen("/tmp/ctodo.bin", "rb");
    mu_check(fp != NULL);

    for (int i = 0; i < 3; i++) {
        struct TodoItem *read_item = malloc(sizeof(struct TodoItem));
        mu_check(read_item_from_stream(read_item, fp) == EXECUTION_SUCCESS);

        mu_check(read_item->identifier == identifiers[i]);
        mu_check(read_item->status == statuses[i]);

        if (texts[i] == NULL) {
            mu_check(strlen(read_item->text) == 0);
        } else {
            mu_check(strcmp(read_item->text, texts[i]) == 0);
        }

        free(read_item);
    }

    unlink("/tmp/ctodo.bin");
}

MU_TEST_SUITE(item_read_write_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_read_write_single_item);
    MU_RUN_TEST(test_read_write_multiple_items);
}
#endif
