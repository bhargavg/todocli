#ifndef _OPTIONS_H__
#define _OPTIONS_H__

#include <stdbool.h>

#define OPTIONS_FLAG_UNSPECIFIED 0
#define OPTIONS_FLAG_ALL         (1 << 0)
#define OPTIONS_FLAG_PENDING     (1 << 1)
#define OPTIONS_FLAG_COMPLETED   (1 << 2)
#define OPTIONS_FLAG_SUMMARY     (1 << 3)
#define OPTIONS_FLAG_HELP        (1 << 4)
#define IS_FLAG_SET(options, option_flag) (options.flags & option_flag)
#define SET_FLAG(options, option_flag) (options.flags |= option_flag)
#define CLR_FLAG(options, option_flag) (options.flags &= ~option_flag)
#define CLR_ALL_FLAGS(options) (options.flags = OPTIONS_FLAG_UNSPECIFIED)

struct Options {
    char *dir_path;
    char *file_path;

    unsigned int flags;

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
