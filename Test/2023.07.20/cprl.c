#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Function to recursively copy directories and create hard links for files
void copy_directory(const char *source, const char *dest) {
    DIR *dir = opendir(source);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    struct dirent *entry;

    // Create destination directory with the same permissions as the source
    if (stat(source, &statbuf) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (mkdir(dest, statbuf.st_mode) == -1) {
        if (errno != EEXIST) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[PATH_MAX];
        char dest_path[PATH_MAX];

        // Construct full source and destination paths
        snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

        if (stat(source_path, &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // Recursively copy directories
        if (S_ISDIR(statbuf.st_mode)) {
            copy_directory(source_path, dest_path);
        } 
        // Create hard link for files
        else if (S_ISREG(statbuf.st_mode)) {
            if (link(source_path, dest_path) == -1) {
                perror("link");
                exit(EXIT_FAILURE);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    copy_directory(argv[1], argv[2]);

    return 0;
}
