#include "package_xbps.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>

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
