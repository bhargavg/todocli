#include <stdio.h>
#include "cmd-list.h"
#include "common.h"

int run_add(int argc, char *argv[]) {

    printf("In list sub command with args: \n");

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return 123;
};

const struct SubCommand list_subcommand = {
    "list",
    run_add
};

