#include "args.h"

const char *argp_program_version = "0.1.0";
const char *argp_program_bug_address = "<igor.botukhov@gmail.com>";

static char doc[] = "Whatever. Run it in wm, it could break your system";

static char args_doc[] = "Docs ahaah";


static struct argp_option options[] = {
    {"init", 'i', "INIT",        0, "Init the system"},
    {"sync", 'w', "SYNCHRONIZE", 0, "Sync everything"},
    {"show", 's', "SHOW",        0, "Show the system"},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'i':
            arguments->init_arg = arg;
            break;
        case 'w':
            arguments->sync_arg = arg;
            break;
        case 's':
            arguments->show_arg = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

void parse_args(int argc, char **argv, struct arguments *args){
    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (args->init_arg){
        printf("Init: %s\n", args->init_arg);
    }

    if (args->sync_arg){
        printf("Sync: %s\n", args->sync_arg);
    }

    if (args->show_arg){
        printf("Show: %s\n", args->show_arg);
    }
}
