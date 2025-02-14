// Use threads, received as program arguments (first parameter total threads, second parameter maximum concurrent threads) and the
// last program argument should be a file. Read the number of vowels in the file using threads.

// ./exec 10 7 read.txt

// PROBABLY WONT BE ON THE EXAM
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE 2048

// STEP 1: Declare global variables
long total_vowels = 0;
pthread_mutex_t mutex;
sem_t semaphore;

typedef struct data {
    long start;
    long end;
    int fd;
} data;

int is_vowel(char c){
    return strchr("AEIOUaeiou", c) != NULL;
}

void *count_vowels(void *arg){
    sem_wait(&semaphore);
    data *d = (data*)arg;

    char buffer[SIZE];
    long local_count = 0;

    long start = d->start;
    long end = d->end;

    int fd = d->fd;
    lseek(fd, start, SEEK_SET);
    long bytes_read = end - start;

    // THIS SECTION IS WRONG
    while (bytes_read > 0){
        long chunk_size = (bytes_read < SIZE) ? bytes_read : SIZE;
        ssize_t bytes = read(fd, buffer, chunk_size);
        if (bytes <= 0) break;
        bytes_read -= bytes;
        for (ssize_t i = 0 ; i < bytes_read; i++) {
            printf("%c", buffer[i]);
            if (is_vowel(buffer[i])) local_count++;
        }
    }
    // UNTIL HERE

    pthread_mutex_lock(&mutex);
    total_vowels += local_count;
    pthread_mutex_unlock(&mutex);
    printf("\nTHREAD: %ld\n", local_count);

    sem_post(&semaphore);
    free(d);
    return NULL;
}

int main(int argc, char **argv){
    if (argc != 4){
        perror("Could not execute program, too many / too few arguments!\n");
        exit(1);
    }

    char *file_path = argv[1];
    int num_threads = atoi(argv[2]);
    int max_concurrent = atoi(argv[3]);

    int fd = open(file_path, O_RDONLY);
    if (fd < 0){
        perror("Error opening!\n");
        return 1;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        perror("Error getting size!\n");
        close(fd);
        exit(1);
    }
    long file_size = file_stat.st_size;

    pthread_t threads[num_threads];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semaphore, 0, max_concurrent);

    long last_start = 0;
    for (int i = 0; i < num_threads ; i++){
        data *d = malloc(sizeof(data));
        d->fd = fd;
        d->start = last_start;
        d->end = (i == num_threads - 1) ? file_size : last_start + file_size / num_threads;

        pthread_create(&threads[i], NULL, count_vowels, d);
    }

    for (int i = 0 ; i < num_threads ; i++) pthread_join(threads[i], NULL);
    printf("Total count: %ld\n", total_vowels);

    close(fd);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

}