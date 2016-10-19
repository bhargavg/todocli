#ifndef _ITEM_H__
#define _ITEM_H__

#include <stdbool.h>
#include <string.h>

enum ItemStatus {
    NOT_COMPLETED = 0,
    COMPLETED = 1,
    REMOVED = 2
};

struct TodoItem {
    enum ItemStatus status;
    char *text;
};

void serialize_item_to_stream(const struct TodoItem item, FILE *fp);
int deserialize_item_from_stream(struct TodoItem *item, FILE *fp);
bool is_item_completed(struct TodoItem);
bool is_item_removed(struct TodoItem);


#endif
