#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "cmd-list.h"
#include "libtodo.h"
#include "../common.h"
#include "../argument-parser.h"

void print_list_help(FILE *fp);

void print_all(struct TodoListMetadata metadata);
void print_items_with_status(struct TodoListMetadata metadata, enum ItemStatus status);
void print_item(struct TodoItem item);
void print_summary(struct TodoListMetadata metadata);

int run_list(struct Options *options, struct TodoListMetadata *metadata) {
    if (options->pending) {
        print_items_with_status(*metadata, NOT_COMPLETED);
    } else if (options->completed) {
        print_items_with_status(*metadata, COMPLETED);
    } else if (options->summary) {
        print_summary(*metadata);
    } else {
        print_all(*metadata);
    }

    return EXECUTION_SUCCESS;
};

void print_summary(struct TodoListMetadata metadata) {
    unsigned long int pending_items_count = 0, completed_items_count = 0;
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];
        if (item->status == COMPLETED) {
            completed_items_count++;
        } else if (item->status == NOT_COMPLETED) {
            pending_items_count++;
        }
    }

    printf("You have %lu pending item(s) and completed %lu item(s)\n", pending_items_count, completed_items_count);
}

void print_all(struct TodoListMetadata metadata) {
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];
        print_item(*item);
    }
}

void print_items_with_status(struct TodoListMetadata metadata, enum ItemStatus status) {
    for (unsigned long int i = 0; i < metadata.items_count; i++) {
        struct TodoItem *item = metadata.items[i];

        if (item->status == status) {
            print_item(*item);
            printf("\n");
        }
    }
}

void print_item(struct TodoItem item) {
    char *status = (item.status == COMPLETED) ? "✔" : ((item.status == REMOVED) ? "✘" : " ");
    printf("%s %lu: %s", status, item.identifier, item.text);
}


const struct SubCommand list_subcommand = {
    .name = "list",
    .description = "Print all the todo items",
    .run = run_list,
    .help_printer = print_list_help
};

void print_list_help(FILE *fp) {
    fprintf(fp, "Usage: todo list\n\n");
    fprintf(fp, "Description: %s\n", list_subcommand.description);
}

