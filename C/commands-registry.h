#ifndef _COMMANDS_REGISTRY_H__
#define _COMMANDS_REGISTRY_H__

/*
struct SubCommand find_sub_command_with_name(struct SubCommand registry[], int no_of_commands, char *name);
struct SubCommand get_sub_command(int argc, char *argv[], struct SubCommand registry[], size_t no_of_sub_commands, struct SubCommand default_sub_command);
*/


void get_sub_command_args(int argc, char *argv[], char **sub_command_name, int *s_argc, char ***s_argv);

#endif
