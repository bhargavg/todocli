#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd-list.h"

#define EXIT_INVALID_SUBCOMMAND 2

enum item_status {
    NOT_COMPLETED = 0,
    COMPLETED
};

void usage() {
    FILE *fp = stderr;

    fprintf(fp,
      "Usage: todo [SUB_COMMAND] [ARGUMENTS]\n\n"
      "Sub commands:\n"
      "  list: Print all the todo items\n"
      "   add: Add a given item to the todo list\n"
      "    rm: Remove the todo item\n"
      "  mark: Mark an item as completed\n\n"
    );
}

int main(int argc, char *argv[]) {
    struct SubCommand registry[] = {
        list_subcommand
    };

    char *sub_command_name = list_subcommand.name;
    if (argc > 1) {
        sub_command_name = argv[1];
    }


    char *sub_command_args[abs(argc - 2)];
    for (int i = 2; i < argc; i++) {
        sub_command_args[i - 2] = argv[i];
    }

    for (int i=0; i < sizeof(registry)/sizeof(registry[0]); i++) {
        struct SubCommand sub_command = registry[i];

        if (strcmp(sub_command.name, sub_command_name) == 0) {
            return sub_command.run(argc - 2, sub_command_args);
        }
    }

    printf("Couldn't get sub command");

    return EXIT_SUCCESS;
}

