#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Fuc. to determine type of file (script o ELF executable)
const char* get_executable_type(const char *filepath) {
    // Open file in binary read
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Error opening file");
        return "unknown";
    }

    // Buffer that reads first 4 byte of file
    char buffer[5] = {0};  
    fread(buffer, 1, 4, file);  
    fclose(file);  

    // Check if is a  ELF
    if (memcmp(buffer, "\x7f""ELF", 4) == 0) {
        return "ELF executable";
    } 
    // Check if is a script (starts with #!)
    else if (memcmp(buffer, "#!", 2) == 0) {
        return "script";
    } 
    else {
        return "unknown";  
    }
}

// Func. to search in subtree
void search_executables(const char *dir_path, const char *filename) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);  

    if (dp == NULL) {
        perror("Errore nell'apertura della directory");
        return;
    }

    // Reads every element of dir
    while ((entry = readdir(dp)) != NULL) {
        // Ignore special file "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        // Build full path
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            perror("Errore nell'ottenere le informazioni del file");
            continue;
        }

        // if a dir call recursively search_executables
        if (S_ISDIR(statbuf.st_mode)) {
            search_executables(path, filename);
        } 
        // if a file
        else if (S_ISREG(statbuf.st_mode) && strcmp(entry->d_name, filename) == 0) {
            // Check if an exec.
            if (access(path, X_OK) == 0) {
                // Print path + type exec.
                printf("%s: %s\n", path, get_executable_type(path));
            }
        }
    }

    closedir(dp);  
}

int main(int argc, char *argv[]) {
    // Check if exaclty 1 param passed (file to search)
    if (argc != 2) {
        fprintf(stderr, "ERROR, correct usage: %s <name_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    search_executables(".", argv[1]);

    return 0;
}
