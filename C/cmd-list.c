#include <stdio.h>
#include "cmd-list.h"
#include "common.h"

int run_list(int argc, char *argv[]) {

    printf("In list sub command with args: \n");

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return EXECUTION_SUCCESS;
};

const struct SubCommand list_subcommand = {
    "list",
    run_list
};

