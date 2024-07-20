#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char **vreaddir(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error in opening directory");
        return NULL;
    }

    struct dirent *entry;
    // count files
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        count++;
    }

    // get back to the start of dir
    rewinddir(dir);

    // allocate string vector
    char **file_list = malloc((count + 1) * sizeof(char *));
    if (!file_list) {
        perror("Error in allocating memory");
        closedir(dir);
        return NULL;
    }

    // fill vector with file name
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        file_list[i] = strdup(entry->d_name);
        if (!file_list[i]) {
            perror("Error in duplicating string");
            for (int j = 0; j < i; j++) {
                free(file_list[j]);
            }
            free(file_list);
            closedir(dir);
            return NULL;
        }
        i++;
    }
    file_list[i] = NULL;

    closedir(dir);
    return file_list;
}

int main(int argc, char *argv[]) {
    // reading dir. -> current
    const char *dir_path = (argc > 1) ? argv[1] : ".";

    char **file_list = vreaddir(dir_path);
    if (!file_list) {
        fprintf(stderr, "Failed to read directory %s\n", dir_path);
        return EXIT_FAILURE;
    }

    printf("Files in directory %s:\n", dir_path);
    for (int i = 0; file_list[i] != NULL; i++) {
        printf("%s\n", file_list[i]);
        free(file_list[i]);  
    }
    free(file_list);  

    return EXIT_SUCCESS;
}
