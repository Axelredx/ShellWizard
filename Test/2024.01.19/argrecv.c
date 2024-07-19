#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_ARGS 100 

int main() {
    char buffer[4096];
    ssize_t bytes_read;
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *ptr = buffer;
    
    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Errore nella lettura dello standard input");
        exit(EXIT_FAILURE);
    }

    // buffer need to ends with null/'\0'
    buffer[bytes_read] = '\0'; 

    // Divide buffer in arguments
    while (ptr < buffer + bytes_read) {
        args[arg_count++] = ptr;
        // goto next arg if '\0'
        ptr += strlen(ptr) + 1; 
        if (arg_count >= MAX_ARGS) {
            fprintf(stderr, "Errore: troppi argomenti\n");
            exit(EXIT_FAILURE);
        }
    }

    // ends array with NULL
    args[arg_count] = NULL; 
    execvp(args[0], args);

    perror("Errore in execvp");
    exit(EXIT_FAILURE);
}
