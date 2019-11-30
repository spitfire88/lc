#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * int pthread_create(pthread_t *thread, pthread_attr_t *attr, 
 * 		void *(*start_routine)(void *), void *arg);
 * 
 * void pthread_exit(void *retval);
 * int pthread_join(pthread_t th, void **thread_return);
*/
#include <pthread.h>

void *thread_func(void *arg);
int COUNTER = 0;
int *message = &COUNTER;

int main() {
	int res;
	pthread_t a_th;
	void *th_res;

	/* typecast message since definition expects void ptr */
	res = pthread_create(&a_th, NULL, thread_func, (void *)message);
	if (res != 0) {
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for thread to finish up....");
	res = pthread_join(a_th, &th_res);
	if (res != 0) {
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined, it returned %s\n", (char *)th_res);
	printf("Message is now %d@0X%x\n", *message, message);
	exit(EXIT_SUCCESS);
}

void *thread_func(void *arg) {
	printf("Thread function is now running. Argument was %d@0X%x\n", *(int *)arg, (int *)arg);
	sleep(5-*(int *)arg);
	//strcpy(message, "Bye!");
	++*message;
	pthread_exit("Thank you for the CPU time");
}
