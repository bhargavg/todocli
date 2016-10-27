#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "helpers.h"
#include "../libtodo.h"

void remove_directory(char *dir_path) {
    if (rmdir(dir_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo directory");
    }
}

void remove_file(char *file_path) {
    if (unlink(file_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo file");
    }
}

void create_directory(char *dir_path) {
    if (mkdir(dir_path, 0755) != 0
        && errno != EEXIST) {
        die("unable to create todo directory");
    }
}
void die(char *text) {
    perror(text);
    exit(1);
}

void create_todo_file_with_metadata(char *file_path, unsigned long int version, unsigned long int items_count) {
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL) {
        die("unable to open todo file for writing");
    }

    if (fwrite(&version, metadata_version_string_byte_count, 1, fp) != 1) {
        die("unable to write the contents to todo file");
    }

    if (fwrite(&items_count, metadata_items_byte_count, 1, fp) != 1) {
        die("unable to write the contents to todo file");
    }

    fclose(fp);
}
