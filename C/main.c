#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "cmd-list.h"
#include "cmd-add.h"
#include "cmd-init.h"
#include "cmd-tick.h"
#include "cmd-untick.h"
#include "cmd-rm.h"

void usage(const struct SubCommand sub_commands[], int length) {
    FILE *fp = stderr;

    fprintf(fp,
      "Usage: todo [SUB_COMMAND] [ARGUMENTS]\n\n"
      "Sub commands:\n"
    );

    for (int i = 0; i < length; i++) {
        const struct SubCommand sub_command = sub_commands[i];
        fprintf(fp, "%s: %s\n", sub_command.name, sub_command.description);
    }
}

int main(int argc, char *argv[]) {

    struct SubCommand registry[] = {
        init_subcommand,
        add_subcommand,
        list_subcommand,
        tick_subcommand,
        untick_subcommand,
        rm_subcommand
    };

    char *sub_command_name = list_subcommand.name;
    if (argc > 1) {
        sub_command_name = argv[1];
    }

    char *sub_command_args[abs(argc - 2)];
    for (int i = 2; i < argc; i++) {
        sub_command_args[i - 2] = argv[i];
    }

    if (!is_initialized() 
         && strcmp(sub_command_name, init_subcommand.name) != 0) {
        printf("Not initialized. Please initialize todo with \"todo init\"\n");
        return NOT_INITIALIZED;
    }

    for (int i=0; i < sizeof(registry)/sizeof(registry[0]); i++) {
        struct SubCommand sub_command = registry[i];

        if (strcmp(sub_command.name, sub_command_name) == 0) {
            return sub_command.run(argc - 2, sub_command_args);
        }
    }

    printf("error: Unknown command - %s\n\n", sub_command_name);
    usage(registry, 3);

    return EXIT_SUCCESS;
}

