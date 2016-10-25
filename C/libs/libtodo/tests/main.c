#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../libtodo.h"
#include "libs/minunit.h"
#include "initialization-tests.h"

int main(int argc, char *argv[]){
    MU_RUN_SUITE(initialization_test_suite);
    MU_REPORT();

    return 0;
}
