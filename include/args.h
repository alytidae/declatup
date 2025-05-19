#ifndef ARGS_H
#define ARGS_H

#include <argp.h>

struct arguments {
    char *init_arg;
    char *sync_arg;
    char *show_arg;
};

void parse_args(int argc, char **argv, struct arguments *args);

#endif
