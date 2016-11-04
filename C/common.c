#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "libtodo.h"
#include "common.h"
#include "commands/cmd-init.h"


char *get_default_todo_directory() {
    const char *homedir = getenv("HOME");
    if (homedir == NULL) {
        die(2, "HOME environment variable not set");
    }

    char *directory = "/.todo/";
    char *path = (char *)malloc(strlen(homedir) + strlen(directory) + 1);
    strcpy(path, homedir);
    strcat(path, directory);

    return path;
}

int sanitized_index_arg_value(char *arg, unsigned long int *index) {
    unsigned long int value = strtoul(arg, NULL, 10);
   *index = value;
   return EXECUTION_SUCCESS;
}

// From: http://stackoverflow.com/a/4770992
bool has_prefix(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}


void die(int status, char *message) {
    char *prefix = "error: ";
    char *normalized_message = (message != NULL) ? message : "";

    const int buffer_length = 500;
    char buffer[buffer_length];

    sprintf(buffer, "error: %s\n", normalized_message);

    perror(buffer);
    exit(status);
}
