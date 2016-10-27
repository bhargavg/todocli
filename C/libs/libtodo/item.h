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
    unsigned long int identifier;
    enum ItemStatus status;
    char *text;
};

int write_item_to_stream(struct TodoItem item, FILE *fp);
int read_item_from_stream(struct TodoItem *item, FILE *fp);

int update_status(struct TodoItem item, FILE *fp);
int item_with_identifier(unsigned long int identifier, struct TodoItem *item, FILE *fp);

int seek_to_item_with_identifier(unsigned long int identifier, FILE *fp);
int seek_to_first_item(FILE *fp);

bool is_item_completed(struct TodoItem);
bool is_item_removed(struct TodoItem);

#endif
