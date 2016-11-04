#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "cmd-init.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

void print_init_help(FILE *);

int run_init(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->values_count > 0) {
        printf("\"init\" doesnot take any parameters\n");
        print_init_help(stdout);
        return UNKNOWN_ERROR;
    }

    return initialize(options->dir_path);
};


const struct SubCommand init_subcommand = {
    .name = "init",
    .description = "initialize todo",
    .run = run_init,
    .help_printer = print_init_help
};

void print_init_help(FILE *fp) {
    fprintf(fp, "Usage: todo init\n\n");
    fprintf(fp, "Description: %s\n", init_subcommand.description);
}
