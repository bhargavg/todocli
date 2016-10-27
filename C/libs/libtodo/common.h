#ifndef _LIB_TODO_COMMON_H__
#define _LIB_TODO_COMMON_H__

#include "libtodo.h"

unsigned long int available_items_count(struct TodoListMetadata *metadata);
int write_metadata(struct TodoListMetadata *metadata, FILE *fp);
int read_metadata(struct TodoListMetadata **metadata, FILE *fp);
int read_item_from_stream(struct TodoItem **item, FILE *fp);
bool is_file_empty(FILE *fp);

#endif
