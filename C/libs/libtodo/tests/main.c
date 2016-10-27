#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../libtodo.h"
#include "libs/minunit.h"
#include "initialization-test-suite.h"
#include "item-read-write-test-suite.h"
#include "item-status-update-test-suite.h"
#include "item-removal-test-suite.h"
#include "helpers.h"

char *dir_path = "/tmp/ctodo/";
char *file_path = "/tmp/ctodo/todo.bin";

int main(int argc, char *argv[]){
    MU_RUN_SUITE(initialization_test_suite);
    MU_RUN_SUITE(item_read_write_test_suite);
    MU_RUN_SUITE(item_status_update_test_suite);
    MU_RUN_SUITE(item_removal_test_suite);
    MU_REPORT();

    return 0;
}
