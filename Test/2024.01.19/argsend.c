#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h> 
#include <string.h>  

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s arg1 arg2 ... argN\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Calc. total length of string + 1 ('\0') for every arg
    int total_length = 0;
    for (int i = 1; i < argc; i++) {
        total_length += strlen(argv[i]) + 1;  
    }

    char *sequence = (char *)malloc(total_length);
    if (!sequence) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // Concatenate param in seq.
    char *ptr = sequence;
    for (int i = 1; i < argc; i++) {
        strcpy(ptr, argv[i]);
        ptr += strlen(argv[i]);
        *ptr = '\0';  
        ptr++;
    }

    write(STDOUT_FILENO, sequence, total_length);

    free(sequence);

    return 0;
}
