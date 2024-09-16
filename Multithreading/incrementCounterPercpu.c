/**
 * @brief The maximum number of CPUs that can be specified in a CPU set.
 *
 * CPU_SETSIZE is a constant defined in the <sched.h> header file. It represents the maximum number of CPUs that can be specified in a CPU set. The value of CPU_SETSIZE may vary depending on the system.
 *
 * In the given code, the percpu_int array is declared with a size of CPU_SETSIZE. This array is used to store the count of increments performed by each thread on their respective CPUs. The value of CPU_SETSIZE determines the size of the percpu_int array and ensures that it can accommodate the count for all possible CPUs in the system.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sched.h>

#define NUM_THREADS 4
#define COUNT 1000
#define CPU_SETSIZE 16

int percpu_int[CPU_SETSIZE];

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    struct timeval start_time, end_time;
    int cpu;

    gettimeofday(&start_time, NULL);
    cpu = sched_getcpu();

    for (int i = 0; i < COUNT; i++) {
        percpu_int[cpu]++;
    }

    gettimeofday(&end_time, NULL);

    long long start_usec = start_time.tv_sec * 1000000LL + start_time.tv_usec;
    long long end_usec = end_time.tv_sec * 1000000LL + end_time.tv_usec;
    double duration = (end_usec - start_usec) / 1000000.0;

    printf("Thread %d ran for %.6f seconds on CPU %d\n", thread_id, duration, cpu);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < CPU_SETSIZE; i++) {
        printf("percpu_int[%d] = %d\n", i, percpu_int[i]);
    }

    return 0;
}