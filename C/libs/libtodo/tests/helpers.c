void remove_todo_directory() {
    remove_todo_file();

    if (rmdir(dir_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo directory");
    }
}

void remove_todo_file() {
    if (unlink(file_path) != 0
        && errno != ENOENT) {
        die("unable to remove todo file");
    }
}

void create_todo_directory() {
    if (mkdir(dir_path, 0755) != 0
        && errno != EEXIST) {
        die("unable to create todo directory");
    }
}

void create_todo_directory_with_todo_file(unsigned long int version, unsigned long int items_count) {
    create_todo_directory();
    create_todo_file_with_metadata(version, items_count);
}

void create_todo_file_with_metadata(unsigned long int version, unsigned long int items_count) {
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
