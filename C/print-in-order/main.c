#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_barrier_t bar1, bar2;

typedef struct {
    // User defined data may be declared here.
    int val;
} Foo;

Foo* fooCreate() {
    Foo* obj = (Foo*) malloc(sizeof(Foo));
    
    // Initialize user defined data here.
    obj->val = 1;

    // Create a barrier
    pthread_barrier_init(&bar1, NULL, 2);
    pthread_barrier_init(&bar2, NULL, 2);
    return obj;
}

void printFirst(void ) {
    printf("First");
}

void printSecond(void ) {
    printf("Second");
}

void printThird(void ) {
    printf("Third");
}

void first(Foo* obj) {
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    pthread_barrier_wait(&bar1);
}

void second(Foo* obj) {
    pthread_barrier_wait(&bar1);
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    pthread_barrier_wait(&bar2);
}

void third(Foo* obj) {
    
    pthread_barrier_wait(&bar2);
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
}

void fooFree(Foo* obj) {
    // User defined data may be cleaned up here.
    free(obj);    
}

int main() {
    int res;
    unsigned int i = 0, count = 0;
    pthread_t a_t, b_t, c_t;
    void *res_t;
    Foo *f;
    int input[] = {3, 2, 1, 
	           1, 2, 3,
		   1, 3, 2,
		   2, 1, 3,
		   2, 3, 1, 
		   3, 1, 2};

    f = fooCreate();
    for (; i < sizeof(input) / sizeof(input[0]); i++) {
	//printf("switch case %d\n", input[i]);
        switch (input[i]) {
	    case 1:
		res = pthread_create(&a_t, NULL, first, NULL);
		if (res != 0) {
			perror("Thread create failed");
			exit(EXIT_FAILURE);
	        }
		break;
	    case 2:
		res = pthread_create(&b_t, NULL, second, NULL);
		if (res != 0) {
			perror("Thread create failed");
			exit(EXIT_FAILURE);
	        }
		break;
	    case 3:
		res = pthread_create(&c_t, NULL, third, NULL);
		if (res != 0) {
			perror("Thread create failed");
			exit(EXIT_FAILURE);
	        }
		break;
	}

	/* Join threads after a trio of each are created */
	if (count == 2) {
		res = pthread_join(a_t, NULL);
		if (res != 0) {
		    perror("Thread join failed");
		    exit(EXIT_FAILURE);
		}
		res = pthread_join(b_t, NULL);
		if (res != 0) {
		    perror("Thread join failed");
		    exit(EXIT_FAILURE);
		}
		res = pthread_join(c_t, NULL);
		if (res != 0) {
		    perror("Thread join failed");
		    exit(EXIT_FAILURE);
		}
		count = 0;
	} else {
		count++;
	}
    }
    printf("\n");
    fooFree(f);
    exit(EXIT_SUCCESS);
}
