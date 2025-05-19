#ifndef ARGS_H
#define ARGS_H

#include <argp.h>

struct arguments {
    int init_flag;
    int sync_flag;
    int show_flag;
};

void parse_args(int argc, char **argv, struct arguments *args);

#endif
