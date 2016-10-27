#ifndef _LIB_TODO_H__
#define _LIB_TODO_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const char *todo_file_name = "todo.bin";
static const int metadata_item_id_byte_count = 4;
static const int metadata_item_status_byte_count = 1;
static const int metadata_item_text_length_byte_count = 4;
static const int metadata_items_byte_count = 4;
static const int metadata_version_string_byte_count = 4;

static const unsigned long int metadata_current_version = 1;

enum TODO_COMMAND_STATUS {
    EXECUTION_SUCCESS = 0,
    UNKNOWN_ERROR = 1,
    NOT_INITIALIZED = 2,
    ALREADY_INITIALIZED = 3,
    TODO_FILE_NOT_FOUND = 4,
    INVALID_TODO_FILE = 5,
    TODO_FILE_WRITE_ERROR = 6,
    TODO_FILE_READ_ERROR = 7,
    TODO_ITEM_NOT_FOUND = 8,
};

enum ItemStatus {
    NOT_COMPLETED = 0,
    COMPLETED = 1,
    REMOVED = 2
};

struct TodoItem {
    unsigned long int identifier;
    enum ItemStatus status;
    char *text;
};

struct TodoListMetadata {
    unsigned long int version;
    unsigned long int items_count;
    struct TodoItem **items;
};

bool is_initialized(char *dir_path);
int initialize(char *dir_path);
int load_metadata(char *dir_path, struct TodoListMetadata **);
int add_item(struct TodoItem *item, struct TodoListMetadata *metadata);
struct TodoItem *item_with_identifier(unsigned long int identifier, struct TodoListMetadata *metadata);
int write_to_stream(struct TodoListMetadata *metadata, FILE *fp);

struct TodoItem *create_todo_item(unsigned long int identifier, enum ItemStatus status, char *text);
void free_todo_item(struct TodoItem *item);
void free_todo_metadata(struct TodoListMetadata *data);

#endif
