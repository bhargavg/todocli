#ifndef _ARGUMENT_PARSER_H__
#define _ARGUMENT_PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Options {
    char *dir_path;
    char *file_path;

    bool all;
    bool pending;
    bool completed;
    bool summary;

    char **values;
    int values_count;
};

struct Argument {
    char *long_name;
    char *short_name;
    bool is_flag;
    int (*parser) (char *value, struct Options *);
    bool (*is_valid)(char *);
};

struct Options *options_new();
void free_options();
void print_options(struct Options options);

int set_base_directory_option(char *value, struct Options *options);
int set_all_option(char *value, struct Options *options);
int set_completed_option(char *value, struct Options *options);
int set_pending_option(char *value, struct Options *options);
int set_summary_option(char *value, struct Options *options);

void process_args(int start_index, int argc, char *argv[], struct Argument args[], int args_count, struct Options *options);

#endif
