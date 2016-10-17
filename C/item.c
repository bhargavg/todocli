#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "common.h"


void serialize_item_to_stream(const struct TodoItem item, FILE *fp) {
    fprintf(fp, "%d,%s", item.status, item.text);
}
