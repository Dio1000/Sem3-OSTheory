// File handling 
/*
1. Check for valid files,
2. Open a file,
3. How to read from files,
4. How to write to files.
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

#define SIZE 1024

// 1. Check for valid files
int valid_file(char *file_path){
    if (access(file_path, F_OK) < 0){
        fprintf(stderr, "File does not exist!\n");
        return 0;
    }

    return 1;
}

int valid_file2(char *file_path){
    int fd = open(file_path, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Cannot read from file!\n");
        return 0;
    }

    return 1;
}

// Program receives 2 arguments, a read file and a write file.
int main(int argc, char **argv){
    if (!valid_file(argv[1]) || !valid_file(argv[2])){
        exit(0);
    }
    
    // int open(const char *path, mode_t)
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        perror("Could not open in read mode!\n");
        exit(1);
    }

    // ssize_t read(int fd, char *buff, int bytes_read)
    char buffer[SIZE + 1];
    ssize_t bytes_read;
    bytes_read = read(fd, buffer, SIZE);
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);

    lseek(fd, 0, SEEK_SET);

    char buffer1[SIZE + 1];
    ssize_t bytes_read1;
    while ((bytes_read1 = read(fd, buffer1, SIZE)) > 0){
        buffer1[bytes_read1] = '\0';
        printf("%s\n", buffer1);
    }

    if (bytes_read1 < 0){
        perror("Could not read from file!\n");
    }

    // test'\0'
    // ^^^^^

    // dafkgjhgdajghdajlhbvjdahbdajhgferuabvjvdanjvda
    // 012345...10

    // dafkgjhgdaj'\0'

    int write_fd = open(argv[2], O_WRONLY | O_TRUNC);
    if (write_fd < 0){
        perror("Could not write to file!\n");
        exit(1);
    }

    // ssize_t write(int fd, char *buff, int bytes_written)
    ssize_t bytes_written;
    bytes_written = write(write_fd, buffer1, strlen(buffer1));
    if (bytes_written < 0){
        perror("Could not write to file!\n");
        exit(1);
    }

    // buffer1 = "Hello", strlen(buffer1) = 5
}