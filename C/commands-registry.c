#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void get_sub_command_args(int argc, char *argv[], char **sub_command_name, int *s_argc, char ***s_argv) {

    // 0       | 1           | 2    | 3    | 4    | 5    | ....
    // --------------------------------------------------------
    // command | sub_command | arg1 | val1 | arg2 | val2 | ....

    if (argc <= 1) {
        *sub_command_name = NULL;
        *s_argc = 0;
        *s_argv = NULL;
        return;
    }

    *sub_command_name = strdup(argv[1]);
    *s_argc = (argc - 2) > 0 ? (argc - 2) : 0;

    *s_argv = malloc(*s_argc * sizeof(char **));

    for (int i = 0; i < *s_argc; i++) {
        *(*s_argv + i) = strdup(argv[i + 2]);
    }
}
