#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 512

// Exercise: Recreate the cpy from UNIX i.e. copy a file to another file
// Example: ./exe file1 file2 will copy the contents of file1 to file2

int main(int argc, char **argv){
    // STEP 0: VALIDATE INPUT
    if (argc != 3) {
        perror("Not enough arguments / Too many arguments!\n");
        exit(1);
    }

    // REMEMBER FLAGS:
    // O_RDONLY -- for reading
    // O_WRONLY -- for writing
    // O_RDWR -- for reading and writing
    // O_TRUNC -- deletes the content before writing
    // O_CREAT -- creates the file if it doesn't exist

    // STEP 1: GET THE FILE DESCRIPTORS (I.E. OPEN THE FILES)
    int read_fd = open(argv[1], O_RDONLY);
    if (read_fd < 0){
        perror("Could not read from file!\n");
        exit(1);
    }

    int write_fd = open(argv[2], O_WRONLY);
    if (write_fd < 0){
        perror("Could not write to file!\n");
        exit(1);
    }

    // STEP 2: READ FROM THE FILE
    char buff[SIZE + 1];
    ssize_t bytes_read;
    while ((bytes_read = read(read_fd, buff, SIZE)) > 0){

        // STEP 3: WRITE TO FILE
        buff[bytes_read] = '\0';
        write(write_fd, buff, strlen(buff));
    }

    printf("Successfully copied contents!\n");

    // OPTIONAL STEP 4: CLOSE FILES
    close(read_fd);
    close(write_fd);
}