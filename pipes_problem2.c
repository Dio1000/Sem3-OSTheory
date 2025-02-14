#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int valid_int(char *string){
    int index = 0;
    while (string[index] != '\0'){
        if (!isdigit(string[index])) return 0;
        index++;
    }

    return 1;
}

int main(int argc, char **argv){
    if (!valid_int(argv[1])) {
        perror("Not a valid integer!\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    for (int i = 0 ; i < n ; i++){
        int pid = fork();
        if (pid < 0){
            perror("Could not fork!\n");
            exit(1);
        }
        // CHILD
        else if (pid == 0){
            // even index
            if (i % 2 == 0) printf("My index is even, here is my parent's PID: %d\n", getppid());
            else printf("My index is odd, here it is: %d\n", getpid());
            exit(0);
        }
        wait(NULL);
    }
}