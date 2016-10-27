#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"
#include "commands/cmd-list.h"
#include "commands/cmd-init.h"
#include "commands/cmd-add.h"
#include "commands/cmd-tick.h"
#include "commands/cmd-untick.h"
#include "commands/cmd-rm.h"

char *dir_path  = "/tmp/ctodo/";
char *file_path = "/tmp/ctodo/todo.bin";

void die_if_error(int status) {
    if (status != EXECUTION_SUCCESS) {
        perror("error: unknown error");
        exit(status);
    }
}

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
        list_subcommand,
        add_subcommand,
        tick_subcommand,
        untick_subcommand,
        rm_subcommand,
    };

    char *sub_command_name = list_subcommand.name;
    if (argc > 1) {
        sub_command_name = argv[1];
    }

    char *sub_command_args[abs(argc - 2)];
    for (int i = 2; i < argc; i++) {
        sub_command_args[i - 2] = argv[i];
    }

    if (!is_initialized(dir_path)) {
        if (strcmp(sub_command_name, init_subcommand.name) == 0) {
            return init_subcommand.run(argc - 2, sub_command_args, NULL);
        } else {
            printf("Not initialized. Please initialize todo with \"todo init\"\n");
            return NOT_INITIALIZED;
        }
    }

    struct TodoListMetadata *metadata;

    die_if_error(load_metadata(dir_path, &metadata));

    for (int i=0; i < sizeof(registry)/sizeof(registry[0]); i++) {
        struct SubCommand sub_command = registry[i];

        if (strcmp(sub_command.name, sub_command_name) == 0) {
            sub_command.run(argc - 2, sub_command_args, metadata);
        }
    }

    FILE *fp = fopen(file_path, "wb");
    die_if_error(fp == NULL);
    die_if_error(write_to_stream(metadata, fp));
    fclose(fp);

    /*

    if (is_initialized(dir_path)) {
        die_if_error(load_metadata(dir_path, &metadata));

        printf("Version: %lu, items: %lu\n", metadata->version, metadata->items_count);
        printf("Listing items:\n");

        for (unsigned long int i = 0; i < metadata->items_count; i++) {
            struct TodoItem *item = metadata->items[i];
            printf("%s\n", item->text);
        }
    } else {
        die_if_error(initialize(dir_path));
        die_if_error(load_metadata(dir_path, &metadata));

        struct TodoItem *item = NULL;

        item = create_todo_item(1, COMPLETED, "Oolala");
        add_item(item, metadata);

        item = create_todo_item(2, NOT_COMPLETED, "Oh yeah");
        add_item(item, metadata);

        item = create_todo_item(3, REMOVED, "Ding Dong");
        add_item(item, metadata);

        FILE *fp = fopen(file_path, "wb");
        if (fp == NULL) {
            printf("error: can't open todo.bin file for writing");
            exit(1);
        }

        die_if_error(write_to_stream(metadata, fp));
        fclose(fp);
    }
    */

    free_todo_metadata(metadata);

    return 0;
}

