#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

__thread uint64_t globalCount = 0;

void* threadFunction(void* arg) {
    globalCount++;
    printf("Thread %ld: globalCount = %lu\n", (long)arg, globalCount);
    return NULL;
}

int main() {
    pthread_t threads[2];

    for (long i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, threadFunction, (void*)i);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}