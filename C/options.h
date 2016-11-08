#ifndef _OPTIONS_H__
#define _OPTIONS_H__

#include <stdbool.h>

struct Options {
    char *dir_path;
    char *file_path;

    bool all;
    bool pending;
    bool completed;
    bool summary;
    bool help;

    char **values;
    int values_count;
};

struct Options *options_new();
void free_options();
void print_options(struct Options options);

int set_base_directory_option(char *value, struct Options *options);
int set_all_option(char *value, struct Options *options);
int set_completed_option(char *value, struct Options *options);
int set_pending_option(char *value, struct Options *options);
int set_summary_option(char *value, struct Options *options);
int set_help_option(char *value, struct Options *options);

#endif
