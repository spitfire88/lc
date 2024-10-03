#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define IPI_SIGNAL SIGUSR1

void ipi_handler(int signum) {
    if (signum == IPI_SIGNAL) {
        printf("Processor 2 received IPI command\n");
    }
}

void* processor1_thread(void* arg) {
    printf("Processor 1 sending IPI command to Processor 2\n");
    pthread_kill(*(pthread_t*)arg, IPI_SIGNAL);
    pthread_exit(NULL);
}

void* processor2_thread(void* arg) {
    // Set up the signal handler for IPI
    struct sigaction sa;
    sa.sa_handler = ipi_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(IPI_SIGNAL, &sa, NULL);

    // Wait for IPI signal
    printf("Processor 2 waiting for IPI command\n");
    pause(); // Wait for signal

    pthread_exit(NULL);
}

int main() {
    pthread_t processor1, processor2;

    // Create the processor 2 thread
    pthread_create(&processor2, NULL, processor2_thread, NULL);

    // Create the processor 1 thread and pass processor 2's thread ID
    pthread_create(&processor1, NULL, processor1_thread, &processor2);

    // Wait for the threads to finish
    pthread_join(processor1, NULL);
    pthread_join(processor2, NULL);

    return 0;
}