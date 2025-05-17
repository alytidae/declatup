#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <string.h>

#include "tomlc17.h"

char* available_package_managers[] = {"xbps", "appimage", "pacman", "portage", "apt"};

typedef struct {
    char* name;
    char* version;
} Package;

static void error(const char *msg, const char *msg1) {
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    exit(1);
}

void install_package_xbps(const char* name){
    printf("Installing package %s with xbps\n", name);

    pid_t pid = fork();

    if (pid==0) { 
        const char *argv[]={"xbps-install","-Sy", name, NULL};

        #ifdef PROD
        execvp("xbps-install", argv);
        #else
        printf("DEGUB: Installed package %s in debug mode\n", name);
        #endif

        exit(127); 
    }
    else { 
        waitpid(pid,0,0);
    }
}

void removed_package_xbps(const char* name){
    printf("Removing package %s with xbps\n", name);

    pid_t pid = fork();

    if (pid==0) { 
        const char *argv[]={"xbps-remove","-Ry", name, NULL};

        #ifdef PROD
        execvp("xbps-remove", argv);
        #else
        printf("DEGUB: Removed package %s in debug mode\n", name);
        #endif

        exit(127); 
    }
    else { 
        waitpid(pid,0,0);
    }
}

void update_packages_xbps(){
    printf("Updating all packages with xbps\n");

    pid_t pid = fork();

    if (pid==0) { 
        const char *argv[]={"xbps-install","-Suy", NULL};

        #ifdef PROD
        execvp("xbps-install", argv);
        #else
        printf("DEGUB: Update packages in debug mode\n");
        #endif

        exit(127); 
    }
    else { 
        waitpid(pid,0,0);
    }
}

void collect_dependencies_xbps(const char* name, Package* packages){
    FILE *fp;
    char buffer[4096];

    char command[256];
    snprintf(command, sizeof(command), "xbps-query -Rx %s", name);

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen failed");
        exit(127); 
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf(">>> %s", buffer); 
    }

    pclose(fp);
}

int get_all_packages_xbps(Package* packages){
    FILE *fp;
    char buffer[256];
    int i = 0;

    fp = popen("xbps-query -l", "r");
    if (fp == NULL) {
        perror("popen failed");
        exit(127); 
    }

    // Example:
    // ii git-2.49.0_1                            Git Tree History Storage Tool
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *name = buffer + 3;
        char *stop = name;

        while(*stop != '\0' && *stop != '\n'){
            if (*stop == ' '){
                break;
            }
            stop++;
        }

        char *version = stop;
        while(*version != '-'){
            version--;
        }

        *stop = '\0';
        *version = '\0';
        version = &version[1];

        Package package;
        package.name = strdup(name);
        package.version = strdup(version);
        packages[i++] = package;        
    }

    pclose(fp);
    return i;
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


int main(){
    #ifdef PROD
    if (geteuid() != 0) {
        fprintf(stderr, "This program must be run as root.\n");
        exit(EXIT_FAILURE);
    }
    #else
    printf("DEGUB: Launched in debug mode.\n");
    #endif

    // Parse the toml file
    toml_result_t result = toml_parse_file_ex("setup.toml");

    // Check for parse error
    if (!result.ok) {
        error(result.errmsg, 0);
    }    

    // Extract values
    // toml_datum_t general  = toml_table_find(result.toptab, "general");
    // toml_datum_t username = toml_table_find(general, "username");
    // toml_datum_t hostname = toml_table_find(general, "hostname");
    // toml_datum_t init     = toml_table_find(general, "init");
    // toml_datum_t distro   = toml_table_find(general, "distro");

    // if (username.type != TOML_STRING) {
    //     error("missing or invalid 'general.username' property in config", 0);
    // }

    // printf("%s | %s | %s | %s\n", username.u.s, hostname.u.s, init.u.s, distro.u.s);
    
    toml_datum_t all_package_managers = toml_table_find(result.toptab, "pkgm");

    int available_package_managers_size =  sizeof(available_package_managers) / sizeof(available_package_managers[0]);
    for (int i = 0; i < available_package_managers_size; ++i){
        toml_datum_t package_manager = toml_table_find(all_package_managers, available_package_managers[i]);
        if (package_manager.type != TOML_TABLE){
            //TODO: Describe it
            continue;
        }
        toml_datum_t packages = toml_table_find(package_manager, "packages");

        if (packages.type != TOML_ARRAY) {
            error("missing or invalid 'packages' property in config", 0);
        }

        printf("---%s---\n", available_package_managers[i]);

        // TODO: Lol 5k, solve later
        Package dependencies[5000]; 

        for (int j = 0; j < packages.u.arr.size; ++j){
            toml_datum_t elem = packages.u.arr.elem[j];
            if (elem.type != TOML_STRING) {
                error("pkg.packages element not an string", 0);
            }
            
            collect_dependencies_xbps(elem.u.s, dependencies);

            // install_package(available_package_managers[i], elem.u.s);
        }
    }

    toml_free(result);

    return 0;
}
