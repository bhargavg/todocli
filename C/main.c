#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtodo.h"
#include "usage.h"
#include "commands/cmd-list.h"
#include "commands/cmd-init.h"
#include "commands/cmd-add.h"
#include "commands/cmd-tick.h"
#include "commands/cmd-untick.h"
#include "commands/cmd-rm.h"

char *dir_path  = "/tmp/ctodo/";
char *file_path = "/tmp/ctodo/todo.bin";

void flush_items_to_disk(struct TodoListMetadata *metadata, char *file_path);
void die_if_error(int status);
struct SubCommand find_sub_command_with_name(struct SubCommand registry[], size_t no_of_commands, char *name);

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


    struct SubCommand sub_command_to_run = find_sub_command_with_name(registry, 6, sub_command_name);

    if (sub_command_to_run.name) {
        sub_command_to_run.run(argc - 2, sub_command_args, metadata);
        flush_items_to_disk(metadata, file_path);
        free_todo_metadata(metadata);
    } else {
        usage(registry, 6);
    }

    return 0;
}

struct SubCommand find_sub_command_with_name(struct SubCommand registry[], size_t no_of_commands, char *name) {
    struct SubCommand command = {0};

    for (int i=0; i < no_of_commands; i++) {
        struct SubCommand sub_command = registry[i];
        if (strcmp(sub_command.name, name) == 0) {
            command = sub_command;
            break;
        }
    }

    return command;
}

void flush_items_to_disk(struct TodoListMetadata *metadata, char *file_path) {
    FILE *fp = fopen(file_path, "wb");
    die_if_error(fp == NULL);
    die_if_error(write_to_stream(metadata, fp));
    fclose(fp);
}

void die_if_error(int status) {
    if (status != EXECUTION_SUCCESS) {
        perror("error: unknown error");
        exit(status);
    }
}

