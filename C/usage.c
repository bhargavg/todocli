#include "usage.h"
#include "common.h"

void usage(const struct SubCommand registry[], int no_of_commands) {
    FILE *fp = stderr;

    fprintf(fp,
      "Usage: todo [SUB_COMMAND] [ARGUMENTS]\n\n"
      "Sub commands:\n"
    );

    for (int i=0; i < no_of_commands; i++) {
        struct SubCommand sub_command = registry[i];
        fprintf(fp, "  %s: %s\n", sub_command.name, sub_command.description);
    }
}
