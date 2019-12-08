/* A ring buffer is a simple, usually fixed-sized, storage mechanism
 * where contiguous memory is treated as if it is circular, and two
 * index counters keep track of the current beginning and the end of
 * the queue. As array indexing is not circular, the indexes must 
 * wrap around to zero when moved past the end of the array.
 * */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>

#define BUF_LEN 16
int buffer[BUF_LEN];
int in = 0, out = 0;
int buffer_size = 0;

// synchronization
pthread_mutex_t lock = /*Mutex is on the stack*/ PTHREAD_MUTEX_INITIALIZER;
sem_t space, count;

void init() {
	//if the mutex was on the heap then we would have used
	//pthread_mutex_init(&lock, NULL);
	sem_init(&space, 0 , BUF_LEN);
	sem_init(&count, 0 , 0);
}

void enqueue(int value) {
	sem_wait(&space);
	
	pthread_mutex_lock(&lock);
	buffer[(in++) & (BUF_LEN - 1)] = value;
	//buffer[in] = value;
	//in = (in + 1) & (BUF_LEN - 1);
	pthread_mutex_unlock(&lock);

	//printf("DEBUG: buffer[in-1] %d in %d\n", *buffer[in-1], in);

	sem_post(&count);
}

int dequeue(void) {
	int result = -1;
	sem_wait(&count);

	//printf("DEBUG: buffer[out] %d out %d\n", *buffer[out], out);
	pthread_mutex_lock(&lock);
	result = buffer[(out++) & (BUF_LEN - 1)];
	//result = buffer[out];
	//out = (out + 1) & (BUF_LEN - 1);
	pthread_mutex_unlock(&lock);

	//printf("DEBUG: out after dequeue %d\n", out);
	sem_post(&space);
	return result;
}

void destroy() {
	sem_destroy(&space);
	sem_destroy(&count);
	pthread_mutex_destroy(&lock);
}

int main() {
	unsigned int c = 0;
	unsigned int v = 0;

	init();
	while(1) {
		printf("in %d out %d\n", in, out);
		printf("Exit/Enqueue/Dequeue 0/1/2: ");
		scanf("%d", &c);
		switch (c) {
		case 0:
			destroy();
			return 0;
		case 1:
			printf("\nEnter value to enqueue: ");
			scanf("%d", &v);
			enqueue(v);
			break;
		case 2: 
			printf("\nDequeued: %d\n", dequeue());
			break;
		default:
			break;
		}
	}
	return 0;
}
