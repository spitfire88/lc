#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef void (*job_function)(int);

typedef struct {
    job_function func;
    int period;
} Job;

typedef struct {
    Job *jobs;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* create_priority_queue(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->jobs = (Job *)malloc(sizeof(Job) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue *pq, int index) {
    if (index && pq->jobs[(index - 1) / 2].period > pq->jobs[index].period) {
        swap(&pq->jobs[index], &pq->jobs[(index - 1) / 2]);
        heapify_up(pq, (index - 1) / 2);
    }
}

void heapify_down(PriorityQueue *pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->jobs[left].period < pq->jobs[smallest].period)
        smallest = left;

    if (right < pq->size && pq->jobs[right].period < pq->jobs[smallest].period)
        smallest = right;

    if (smallest != index) {
        swap(&pq->jobs[index], &pq->jobs[smallest]);
        heapify_down(pq, smallest);
    }
}

void add_job(PriorityQueue *pq, job_function func, int period) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->jobs = (Job *)realloc(pq->jobs, sizeof(Job) * pq->capacity);
    }
    pq->jobs[pq->size].func = func;
    pq->jobs[pq->size].period = period;
    heapify_up(pq, pq->size);
    pq->size++;
}

Job extract_min(PriorityQueue *pq) {
    Job root = pq->jobs[0];
    pq->jobs[0] = pq->jobs[--pq->size];
    heapify_down(pq, 0);
    return root;
}

void scheduler(PriorityQueue *pq) {
    while (pq->size > 0) {
        Job job = extract_min(pq);
        job.func(pq->size);
        sleep(job.period);
    }
}

// Example job functions
void job1(int pqsize) {
    printf("Job 1 executed. Pqsize %d\n", pqsize);
}

void job2(int pqsize) {
    printf("Job 2 executed. Pqsize %d\n", pqsize);
}

int main() {
    PriorityQueue *pq = create_priority_queue(10);
    add_job(pq, job1, 2);
    add_job(pq, job2, 1);
    add_job(pq, job1, 2);
    add_job(pq, job2, 1);
    add_job(pq, job1, 2);
    add_job(pq, job2, 1);
    add_job(pq, job1, 2);
    add_job(pq, job2, 1);
    add_job(pq, job1, 2);
    add_job(pq, job2, 1);
    scheduler(pq);

    free(pq->jobs);
    free(pq);
    return 0;
}