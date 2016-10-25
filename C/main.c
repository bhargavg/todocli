#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"

int main(int argc, char *argv[]) {

    struct TodoListMetadata *metadata = malloc(sizeof(struct TodoListMetadata));
    if (initialize("/tmp/ctodo/", &metadata) != EXECUTION_SUCCESS) {
        printf("Failed");
    } else {
        printf("Success");
    }

    return 0;
}

