#ifndef _TEST_HELPERS__
#define _TEST_HELPERS__

void remove_todo_directory(char *dir_path);
void remove_todo_file(char *file_path);
void create_todo_directory(char *dir_path);
void create_todo_directory_with_todo_file(unsigned long int, unsigned long int);
void create_todo_file_with_metadata(unsigned long int, unsigned long int);

#endif
