#ifndef _ITEM_H__
#define _ITEM_H__

#include <stdbool.h>
#include <string.h>

enum ItemStatus {
    NOT_COMPLETED = 0,
    COMPLETED
};

struct TodoItem {
    enum ItemStatus status;
    char *text;
};

void get_todo_items(struct TodoItem **buffer, FILE *fp);
void serialize_item_to_stream(const struct TodoItem item, FILE *fp);

#endif
