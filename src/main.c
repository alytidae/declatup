#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

#include "tomlc17.h"

static void error(const char *msg, const char *msg1) {
  fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
  exit(1);
}

char* available_package_managers[] = {"xbps", "appimage", "pacman", "portage", "apt"};

void install_package(const char *pkgm, const char *package){
    printf("%s\n",package);
    pid_t pid = fork();

    if (pid==0) { /* child process */

        if (pkgm == "xbps"){
            printf("Installing\n");
            char *argv[]={"xbps-install","-Sy", package, NULL};
            execvp("xbps-install", argv);
        }else{
            printf("Package manager is not implemented!\n");
        }

        exit(127); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */
    }
}



int main(){
    // if (geteuid() != 0) {
    //     fprintf(stderr, "This program must be run as root.\n");
    //     exit(EXIT_FAILURE);
    // }

    // Parse the toml file
    toml_result_t result = toml_parse_file_ex("setup.toml");

    // Check for parse error
    if (!result.ok) {
        error(result.errmsg, 0);
    }    

    // Extract values
    toml_datum_t general  = toml_table_find(result.toptab, "general");
    toml_datum_t username = toml_table_find(general, "username");
    toml_datum_t hostname = toml_table_find(general, "hostname");
    toml_datum_t init     = toml_table_find(general, "init");
    toml_datum_t distro   = toml_table_find(general, "distro");

    if (username.type != TOML_STRING) {
        error("missing or invalid 'general.username' property in config", 0);
    }

    printf("%s | %s | %s | %s\n", username.u.s, hostname.u.s, init.u.s, distro.u.s);
    
    toml_datum_t all_package_managers = toml_table_find(result.toptab, "pkgm");

    for (int i = 0; i < 5; ++i){
        toml_datum_t package_manager = toml_table_find(all_package_managers, available_package_managers[i]);
        if (package_manager.type != TOML_TABLE){
            //TODO: Describe it
            continue;
        }
        toml_datum_t packages = toml_table_find(package_manager, "packages");

        if (packages.type != TOML_ARRAY) {
            error("missing or invalid 'packages' property in config", 0);
        }

        printf("%s\n", available_package_managers[i]);

        for (int j = 0; j < packages.u.arr.size; ++j){
            toml_datum_t elem = packages.u.arr.elem[j];
            if (elem.type != TOML_STRING) {
                error("pkg.packages element not an string", 0);
            }
            // install_package("xbps", elem.u.s);

            printf("%s\n", elem.u.s);
        }
    }



    // printf("%s", pkgm.type == TOML_TABLE);

    // toml_datum_t xbps     = toml_table_find(pkg.toptab, "xbps");
    // toml_datum_t packages = toml_table_find(xbps, "packages");

    // if (packages.type != TOML_ARRAY) {
    //     error("missing or invalid 'pkg.packages' property in config", 0);
    // }

    // for (int i = 0; i < packages.u.arr.size; i++) {
    //     toml_datum_t elem = packages.u.arr.elem[i];
    //     if (elem.type != TOML_STRING) {
    //         error("pkg.packages element not an string", 0);
    //     }
    //     // printf("%s\n", elem.u.s);
    //     install_package("xbps", elem.u.s);
    // }

    toml_free(result);

    return 0;
}
