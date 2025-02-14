// Processes 
/*
1. How to create a new process,
2. How do parents and children work,
3. Inter Process Comunication (via pipes).
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE 1024

// NOTE: Check if you write outside 

int main(int argc, char **argv){
    int child_to_parent[2];

    // PIPE BEFORE FORK!!!
    if (pipe(child_to_parent) < 0) {
        perror("Could not pipe!\n");
        exit(1);
    }

    // int fork(void);
    // -- A random integer (the ID of the process) if the process is the parent,
    // -- 0, if the process is the child,
    // -- A negative integer if the fork failed.
    int pid = fork();
    if (pid < 0){
        perror("Could not fork!\n");
        exit(1);
    }
    // CHILD
    if (pid == 0) {
        close(child_to_parent[0]); // Close unused read

        write(child_to_parent[1], "Hello!\n", strlen("Hello\n"));
        close(child_to_parent[1]); // Close unsured write
        exit(0);
    }
    // PARENT
    else {
        close(child_to_parent[1]); // Close unused write

        char buff[SIZE + 1];
        ssize_t bytes_read = read(child_to_parent[0], buff, SIZE);
        if (bytes_read > 0){
            buff[bytes_read] = '\0';
            printf("Parent: %s\n", buff);
        }
        close(child_to_parent[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)){
            printf("$d\n", WEXITSTATUS(status));
        }
    }
}