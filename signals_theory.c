#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// IMPORTANT, you cannot handle SIGSTOP and SIGKILL
// In order to handle SIGSTOP, use SIGTSTP.

void handle_SIGALRM(int sigNo){
    printf("Running...\n");
    alarm(5); // Function that sends SIGALRM once every n seconds (5 in our case)
}

void handle_SIGTERM(int sigNo){
    printf("Caught SIGTERM!\n");
}

int main(){
    // 1. signal(int sigNo, void function)
    signal(SIGALRM, handle_SIGALRM);

    // 2. sigaction(int sigNo, *struct sigaction sa, NULL)
    struct sigaction sa;
    sa.sa_handler = handle_SIGTERM
    sigaction(SIGTERM, &sa, NULL);
}