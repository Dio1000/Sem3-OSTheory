#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

// STEP 1: Initialise: global_total, semaphore, mutex, max number of elements
int max_number = -1;
volatile long global_counter = 0;
sem_t sem;
pthread_mutex_t mut;

typedef struct data{
    long start;
    long end;
} data;

int valid_num(long n){
    if (n <= 2) return 0;
    for (long d = 3 ; d * d <= n ; d++){
        if (n % d == 0) return 0;
    }

    return ((n % 10 == 3) || (n % 10 == 7));
}

// STEP 3: Execute the thread function
void *count(void *arg){
    sem_wait(&sem); // increments the number of concurrent threads
    data* d = (data*)arg;

    long start = d->start;
    long end = d->end;
    free(d);

    long local_count = 0;
    for (long i = start ; i < end; i++){
        if (valid_num(i)) {
            local_count++;
        }
    }

    // critical section
    pthread_mutex_lock(&mut); 
    global_counter += local_count;
    pthread_mutex_unlock(&mut);

    printf("Thread found: %ld\n", local_count);

    sem_post(&sem); // decrements the number of concurrent threads
    return NULL;
}

int main(int argc, char **argv){
    if (argc != 2){
        perror("Not enough arguments / Too many arguments!\n");
        exit(1);
    }

    // STEP 2: initalise semaphore and threads
    max_number = atoi(argv[1]);
    pthread_t threads[4];
    sem_init(&sem, 0, 3);

    int last_start = 0;
    for (int i = 0 ; i < 4 ; i++){
        data *d = malloc(sizeof(data));
        d->start = last_start;
        d->end = (i == 3) ? max_number : last_start + max_number / 4;

        last_start = d->end;
        // void pthread_create(*pthread_t thread, NULL, void* executed_function, struct*)
        pthread_create(&threads[i], NULL, count, d);
    }

    for (int i = 0 ; i < 4 ; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Total number: %ld\n", global_counter);

    pthread_mutex_destroy(&mut);
    sem_destroy(&sem);
}