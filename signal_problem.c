#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigstp(int signum){
    printf("STOP signal received!\n");
}

int main(){
    struct sigaction sa;
    sa.sa_handler = handle_sigstp;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);

    while (1){
        
    }

    return 0;
}