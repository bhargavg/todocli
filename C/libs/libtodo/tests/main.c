#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../libtodo.h"
#include "libs/minunit.h"
#include "initialization-tests.h"
#include "item-read-write-tests.h"
#include "item-status-update-tests.h"

int main(int argc, char *argv[]){
    MU_RUN_SUITE(initialization_test_suite);
    MU_RUN_SUITE(item_read_write_test_suite);
    MU_RUN_SUITE(item_status_update_test_suite);
    MU_REPORT();

    return 0;
}
