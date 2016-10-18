#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "common.h"

void print_init_help(FILE *);

int run_init(int argc, char *argv[]) {
    if (argc > 0) {
        printf("\"init\" doesnot take any parameters\n");
        print_init_help(stdout);
        return INVALID_PARAMETERS;
    }

    printf("Starting init sequence...\n");

    char *directory = todo_dir_path();

    printf("Checking directory at: \"%s\" - ", directory);

    struct stat sb = {0};
    if(stat(directory, &sb) != 0 
        || !S_ISDIR(sb.st_mode)) {

        printf("Not Found\n");
        printf("Creating one... ");

        int ret = mkdir(directory, 0755);

        if (ret != 0) {
            printf("error: %s, failed to create directory at: \"%s\"\n", strerror(errno), directory);
            free(directory);
            return ret;
        }

        printf("Done!\n");
        printf("Initialization complete!\n");
    } else {
        printf("Found\n");
        printf("todo is already initalized\n");
    }

    free(directory);

    return EXECUTION_SUCCESS;
};

bool is_initialized() {
    char *path = todo_dir_path();
    bool initialized = (access(path, R_OK | W_OK) != -1);
    free(path);

    return initialized;
}

const struct SubCommand init_subcommand = {
    .name = "init",
    .description = "Initialize todo",
    run_init
};


void print_init_help(FILE *fp) {
    fprintf(fp, "Usage: todo init\n\n");
    fprintf(fp, "Description: %s\n", init_subcommand.description);
}
