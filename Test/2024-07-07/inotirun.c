#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

// standard max len. for path
#ifndef PATH_MAX
#define PATH_MAX 4096  
#endif

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

// Func. for processing file created given filepath
void process_file(const char *filepath) {
    // open file in 'read mode'
    FILE *file = fopen(filepath, "r");  
    // error if can't open file
    if (!file) {  
        perror("Error in fopen!");  
        return;  
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // array used to save args of comand
    char *args[100];  
    int arg_count = 0;

    // reading iterations on file rows
    while ((read = getline(&line, &len, file)) != -1) {
        // eraese newline char
        line[read - 1] = '\0';  
        // coping row in args array
        args[arg_count] = strdup(line);  
        arg_count++;  
    }

    args[arg_count] = NULL;  
    fclose(file);   
    free(line);  

    if (arg_count > 0) {  
        // create a child process
        pid_t pid = fork();  
        if (pid == 0) {
            // on child process exec comand
            execvp(args[0], args);  
            // if execvp fail then error
            perror("execvp");  
            exit(EXIT_FAILURE);  
        } else if (pid > 0) {
            // on father process
            int status;
            // wait for child to end
            waitpid(pid, &status, 0);  
            // if child end normally
            if (WIFEXITED(status)) {  
                printf("Command executed with exit status %d\n", WEXITSTATUS(status));
            } else {
                printf("Command execution failed\n");
            }
        } else {
            // fork error
            perror("Error in fork!");  
        }
    }

    // free memory
    for (int i = 0; i < arg_count; i++) {  
        free(args[i]);
    }

    // Remove file after exec.
    if (remove(filepath) == 0) {
        printf("File %s removed successfully\n", filepath);
    } else {
        //removing error
        perror("Error in removing file!");  
    }
}

int main(int argc, char *argv[]) {
    // check if only one arg is written 
    if (argc != 2) {  
        fprintf(stderr, "Bad usage: %s <directory>\n", argv[0]);  
        exit(EXIT_FAILURE);  
    }

    // monitored dir
    const char *watched_dir = argv[1];  
    int inotify_fd = inotify_init();  
    // inotify init error
    if (inotify_fd < 0) { 
        perror("Error in inotify_init!");  
        exit(EXIT_FAILURE);  
    }

    // dir monitored
    int wd = inotify_add_watch(inotify_fd, watched_dir, IN_CREATE); 
    // dir monitoring error 
    if (wd < 0) { 
        perror("Error inotify_add_watch!");  
        close(inotify_fd);  
        exit(EXIT_FAILURE);  
    }

    // buffer used to save monitored events of inotify
    char buffer[BUF_LEN];  
    while (1) {  
        int length = read(inotify_fd, buffer, BUF_LEN); 
        // read error 
        if (length < 0) {  
            perror("Error in reading!");  
            close(inotify_fd);  
            exit(EXIT_FAILURE);  
        }

        int i = 0;
        // iteration on events
        while (i < length) {  
            // obtain current event
            struct inotify_event *event = (struct inotify_event *)&buffer[i]; 
            // event has a name file associated 
            if (event->len) {  
                // event is creation of a file
                if (event->mask & IN_CREATE) {  
                    char filepath[PATH_MAX];
                    // reconstruct 'pwd'
                    snprintf(filepath, PATH_MAX, "%s/%s", watched_dir, event->name);  
                    printf("New file created: %s\n", filepath);
                    process_file(filepath);  
                }
            }
            i += EVENT_SIZE + event->len;  
        }
    }

    // close all
    inotify_rm_watch(inotify_fd, wd);  
    close(inotify_fd);  
    return 0;  
}