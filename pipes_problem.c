#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1024

// if exec starts with a vowel, create one subprocess that prints the number of vowels in the path.
// else, create two subprocesses, that print the number of consonants in the path.

int is_vowel(char c){
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') return 1;
    return 0;
}

/*
Checks if the executable starts with a vowel.
*/
int exec_vowel(char *file_path){
    int len = strlen(file_path);
    int index = len - 1;
    while (file_path[index] != '/'){
        index--;
    }

    if (is_vowel(file_path[index + 1])) return 1; 
    return 0;
}

int count_vowels(char *string){
    printf("%s\n", string);
    int index = 0;

    int count = 0;
    while (string[index] != '\0'){
        printf("%c\n", string[index]);
        if (is_vowel(string[index])) count++;
        index++;
    }

    return count;
}

int count_cons(char *string){
    int index = 0;

    int count = 0;
    while (string[index] != '\0'){
        if (!is_vowel(string[index]) && isalpha(string[index])) count++;
        index++;
    }

    return count;
}

int main(int argc, char **argv){

    // VOWEL PATH
    if (exec_vowel(argv[0])){
        int pid = fork();
        if (pid < 0){
            perror("Could not fork!\n");
            exit(1);
        }
        // CHILD
        if (pid == 0){
            printf("Number of vowels: %d\n", count_vowels(argv[0]));
            exit(0);
        }
        // PARENT
        else {
            wait(NULL);
            printf("Testing executable name starting with a vowel: %s\n", argv[0]);
        }
    }
    // CONSONANT PATH
    else {
        int pid1 = fork();
        if (pid1 < 0){
            perror("Could not fork!\n");
            exit(1);
        }
        // CHILD 1
        if (pid1 == 0){
            printf("Number of consonants: %d\n", count_cons(argv[0]));
            exit(0);
        }
        else{
            wait(NULL);
        }

        int pid2 = fork();
        if (pid2 < 0){
            perror("Could not fork!\n");
            exit(1);
        }
        // CHILD 2
        if (pid2 == 0){
            printf("Number of consonants: %d\n", count_cons(argv[0]));
            exit(0);
        }
        else{
            wait(NULL);
            printf("Testing executable name not starting with a vowel: %s\n", argv[0]);
        }
    }

}