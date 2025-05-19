#ifndef PACKAGE_XBPS_H
#define PACKAGE_XBPS_H

typedef struct {
    char* name;
    char* version;
} Package;

void install_package_xbps(const char* name);

void removed_package_xbps(const char* name);

void update_packages_xbps();

void collect_dependencies_xbps(const char* name, Package* packages);

int get_all_packages_xbps(Package* packages);

#endif
