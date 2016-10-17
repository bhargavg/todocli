#ifndef _COMMON_H__
#define _COMMON_H__

#include <stdbool.h>

struct SubCommand {
    char *name;
    int (*run)(int argc, char *argv[]);
};

#endif
