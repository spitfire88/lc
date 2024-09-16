#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h> // Add this line to include the pthread barrier header

pthread_barrier_t barrier;
int loops;
__thread uint64_t globalCount;

void __attribute__((optimize("O0")))
do_lotsa_work(void) {
    int r = rand() % 100000;
    for (int i = 0; i < 100000; i++) {
        if (r % 100000 == 0){
            globalCount++;
        }
    }
}

void *run(void *arg) {
    pthread_barrier_wait(&barrier);
    for (int i = 0; i < loops; i++) {
        do_lotsa_work();
    }
    pthread_barrier_wait(&barrier);
}

int main(int argc, char ** argv) {
    int threads = 16;//atoi(argv[1]);
    loops = 1000;//atoi(argv[2]);
    pthread_barrier_init(&barrier, NULL, threads);
    pthread_t *threadids = malloc(threads * sizeof(pthread_t));

    for (int i = 0; i < threads; i++) {
        pthread_create(&threadids[i], NULL, run, NULL);
    }
    for (int i = 0; i < threads; i++) {
        pthread_join(threadids[i], NULL);
    }
}