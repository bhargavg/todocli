#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"

int main(int argc, char *argv[]) {

    struct TodoListMetadata *metadata;

    if (initialize("/tmp/ctodo/", &metadata) != EXECUTION_SUCCESS) {
        printf("Failed");
    } else {
        printf("Success");
    }

    return 0;
}

