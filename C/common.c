#include <stdio.h>
#include <stdlib.h>
#include "common.h"

char *todo_dir_path() {
    const char *homedir = getenv("HOME");
    if (!homedir) {
        printf("HOME environment variable not set");
        exit(ENV_HOME_NOT_SET);
    }

    char *path = (char *)malloc(strlen(homedir) + 1);
    strcpy(path, homedir);
    strcat(path, "/.todo/");

    return path;
}

char *todo_file_path() {

    char *directory = todo_dir_path();
    //FIXME: path separators issue, not cross platform
    char *file = "todo.txt";
    int length = strlen(directory) + strlen(file);
    char *file_path = (char *)malloc(length + 1);

    //TODO: check if snprintf can be used here
    strcpy(file_path, directory);
    strcat(file_path, file);

    free(directory);

    return file_path;
}
