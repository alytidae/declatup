#include "args.h"

#include <stdlib.h>

const char *argp_program_version = "0.1.0";
const char *argp_program_bug_address = "<igor.botukhov@gmail.com>";

static char doc[] = "Whatever. Run it in wm, it could break your system";

static char args_doc[] = "Docs ahaah";

static struct argp_option options[] = {
    {"init", 'i', 0, 0, "Init the system"},
    {"sync", 's', 0, 0, "Sync everything"},
    {"show", 'w', 0, 0, "Show the system"},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'i':
            arguments->init_flag = 1;
            break;
        case 's':
            arguments->sync_flag = 1;
            break;
        case 'w':
            arguments->show_flag = 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

void parse_args(int argc, char **argv, struct arguments *args){
    argp_parse(&argp, argc, argv, 0, 0, args);

    if (!args->init_flag && !args->sync_flag && !args->show_flag) {
        printf("Default: If this is your first launch, initialize the system.\n");
        argp_help(&argp, stdout, ARGP_HELP_STD_HELP, argv[0]);
        exit(0);
    }

    if (args->init_flag){
        printf("Init\n");
    }

    if (args->sync_flag){
        printf("Sync\n");
    }

    if (args->show_flag){
        printf("Show\n");
    }
}
