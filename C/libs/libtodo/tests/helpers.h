#ifndef _TEST_HELPERS__
#define _TEST_HELPERS__

#include <stdio.h>
#include <stdlib.h>

void create_directory(char *dir_path);
void create_todo_file_with_metadata(char *file_path, unsigned long int, unsigned long int);

void remove_directory(char *dir_path);
void remove_file(char *file_path);

void die(char *text);

#endif
