#ifndef _ITEM_UPDATE_TESTS__
#define _ITEM_UPDATE_TESTS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "../libtodo.h"
#include "../item.h"
#include "libs/minunit.h"

MU_TEST(test_update_status) {
    int identifiers[] = { 200, 0, 12345, 538 };
    enum ItemStatus statuses[] = { COMPLETED, NOT_COMPLETED, NOT_COMPLETED, REMOVED };
    char *texts[] = { "Foo bar baz", NULL, "Hello, world!", "Lorem ipsum" };

    FILE *fp = fopen("/tmp/ctodo.bin", "wb");

    mu_check(fp != NULL);

    for (int i = 0; i < 4; i++) {
        struct TodoItem item = { .identifier = identifiers[i], .status = statuses[i], .text = texts[i]};
        mu_check(write_item_to_stream(item, fp) == EXECUTION_SUCCESS);
    }


    fclose(fp);


    struct TodoItem item = { .identifier = 12345, .status = REMOVED, .text = "Lorem ipsum" };
    struct TodoItem *out_item = malloc(sizeof(struct TodoItem));
    
    fp = fopen("/tmp/ctodo.bin", "rb");
    mu_check(fp != NULL);
    mu_check(update_status(item, fp) == EXECUTION_SUCCESS);
    mu_check(item_with_identifier(12345, out_item, fp) == EXECUTION_SUCCESS);
    mu_check(out_item->identifier == 12345);
    mu_check(out_item->status == REMOVED);

    unlink("/tmp/ctodo.bin");
}

MU_TEST_SUITE(item_status_update_test_suite) {
    // no setup, teardown functions
    MU_SUITE_CONFIGURE(NULL, NULL);

    MU_RUN_TEST(test_update_status);
}
#endif
