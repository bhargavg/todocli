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
#include "argument-parser.h"


void die_if_error(int status);
void flush_items_to_disk(struct TodoListMetadata *metadata, char *file_path);
void get_arguments(struct Argument **arguments, int *count);

int main(int argc, char *argv[]) {

    int s_argc = 0;
    char **s_argv = NULL;
    char *sub_command_name = NULL;

    get_sub_command_args(argc, argv, &sub_command_name, &s_argc, &s_argv);

    struct Argument *all_arguments = NULL;
    int arguments_count = 0;
    get_arguments(&all_arguments, &arguments_count);

    struct Options *options = options_new();
    process_args(s_argc, s_argv, all_arguments, arguments_count, options);

    struct SubCommand registry[] = {
        init_subcommand,
        list_subcommand,
        add_subcommand,
        tick_subcommand,
        untick_subcommand,
        rm_subcommand,
    };

    int no_of_sub_commands = 6;

    struct SubCommand command_to_run = list_subcommand;

    if (sub_command_name) {
        for (int i = 0; i < no_of_sub_commands; i++) {
            struct SubCommand sub_command = registry[i];
            if (strcmp(sub_command_name, sub_command.name) == 0) {
                command_to_run = sub_command;
                break;
            }
        }
    }

    if (!is_initialized(options->dir_path)) {
        if (strcmp(command_to_run.name, init_subcommand.name) == 0) {
            return init_subcommand.run(options, NULL);
        } else {
            printf("Not initialized. Please initialize todo with \"todo init\"\n");
            return NOT_INITIALIZED;
        }
    }

    struct TodoListMetadata *metadata;
    die_if_error(load_metadata(options->dir_path, &metadata));

    if (command_to_run.name) {
        command_to_run.run(options, metadata);
        flush_items_to_disk(metadata, options->file_path);
        free_todo_metadata(metadata);
    } else {
        usage(registry, no_of_sub_commands);
    }

    return 0;
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

void get_arguments(struct Argument **arguments, int *count) {
    int args_count = 5;
    struct Argument *args = malloc(args_count * sizeof(struct Argument));

    args[0].long_name  = strdup("--all");
    args[0].short_name = strdup("-a");
    args[0].is_flag    = true;
    args[0].parser     = set_all_option;
    args[0].is_valid   = NULL;

    args[1].long_name  = strdup("--completed");
    args[1].short_name = strdup("-c");
    args[1].is_flag    = true;
    args[1].parser     = set_completed_option;
    args[1].is_valid   = NULL;

    args[2].long_name  = strdup("--pending");
    args[2].short_name = strdup("-p");
    args[2].is_flag    = true;
    args[2].parser     = set_pending_option;
    args[2].is_valid   = NULL;

    args[3].long_name  = strdup("--summary");
    args[3].short_name = strdup("-s");
    args[3].is_flag    = true;
    args[3].parser     = set_summary_option;
    args[3].is_valid   = NULL;

    args[4].long_name  = strdup("--dir");
    args[4].short_name = strdup("-d");
    args[4].is_flag    = false;
    args[4].parser     = set_base_directory_option;
    args[4].is_valid = NULL;

    *arguments = args;
    *count = args_count;
}
