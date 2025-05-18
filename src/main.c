#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>
#include <argp.h>

#include "tomlc17.h"
#include "package_xbps.h"
#include "args.h"

char* available_package_managers[] = {"xbps", "appimage", "pacman", "portage", "apt"};

static void error(const char *msg, const char *msg1) {
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    exit(1);
}

void install_package(const char *pkgm, const char *name){
    if (strcmp(pkgm, "xbps") == 0){
        install_package_xbps(name);
    }else if (strcmp(pkgm, "appimage") == 0){
        // install_package_appimage(name);
    }else if (strcmp(pkgm, "pacman") == 0){
        // insatll_package_pacman(name);
    }else if (strcmp(pkgm, "portage") == 0){
        // install_package_portage(name);
    }else if (strcmp(pkgm, "apt") == 0){
        // install_package_apt(name);
    }else{
        printf("Package manager is not implemented!\n");
    }
}


int main(int argc, char *argv[]){
    
    struct arguments args = {
        .init_arg = NULL,
        .sync_arg = NULL,
        .show_arg = NULL
    };

    parse_args(argc, argv, &args);

    return 0;

    
    // #ifdef PROD
    // if (geteuid() != 0) {
    //     fprintf(stderr, "This program must be run as root.\n");
    //     exit(EXIT_FAILURE);
    // }
    // #else
    // printf("DEGUB: Launched in debug mode.\n");
    // #endif

    // // Parse the toml file
    // toml_result_t result = toml_parse_file_ex("setup.toml");

    // // Check for parse error
    // if (!result.ok) {
    //     error(result.errmsg, 0);
    // }    

    // // Extract values
    // // toml_datum_t general  = toml_table_find(result.toptab, "general");
    // // toml_datum_t username = toml_table_find(general, "username");
    // // toml_datum_t hostname = toml_table_find(general, "hostname");
    // // toml_datum_t init     = toml_table_find(general, "init");
    // // toml_datum_t distro   = toml_table_find(general, "distro");

    // // if (username.type != TOML_STRING) {
    // //     error("missing or invalid 'general.username' property in config", 0);
    // // }

    // // printf("%s | %s | %s | %s\n", username.u.s, hostname.u.s, init.u.s, distro.u.s);
    
    // toml_datum_t all_package_managers = toml_table_find(result.toptab, "pkgm");

    // int available_package_managers_size =  sizeof(available_package_managers) / sizeof(available_package_managers[0]);
    // for (int i = 0; i < available_package_managers_size; ++i){
    //     toml_datum_t package_manager = toml_table_find(all_package_managers, available_package_managers[i]);
    //     if (package_manager.type != TOML_TABLE){
    //         //TODO: Describe it
    //         continue;
    //     }
    //     toml_datum_t packages = toml_table_find(package_manager, "packages");

    //     if (packages.type != TOML_ARRAY) {
    //         error("missing or invalid 'packages' property in config", 0);
    //     }

    //     printf("---%s---\n", available_package_managers[i]);

    //     // TODO: Lol 5k, solve later
    //     Package dependencies[5000]; 

    //     for (int j = 0; j < packages.u.arr.size; ++j){
    //         toml_datum_t elem = packages.u.arr.elem[j];
    //         if (elem.type != TOML_STRING) {
    //             error("pkg.packages element not an string", 0);
    //         }
            
    //         collect_dependencies_xbps(elem.u.s, dependencies);

    //         // install_package(available_package_managers[i], elem.u.s);
    //     }
    // }

    // toml_free(result);

    return 0;
}
