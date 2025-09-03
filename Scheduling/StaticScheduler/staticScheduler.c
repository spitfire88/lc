#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAX_TASKS 10

typedef struct {
    int id;
    void (*task_function)(void);
    uint32_t delay;         // ms
    uint32_t period;        // ms, 0 for one-shot
    bool is_periodic;
    uint32_t next_run_time; // ms
    bool active;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

// Returns system time in milliseconds since program start
uint32_t get_system_time() {
    static uint32_t start = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t now = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    if (start == 0) start = now;
    return now - start;
}

void add_task(void (*func)(void), uint32_t delay, uint32_t period, bool is_periodic) {
    if (task_count >= MAX_TASKS) return;
    tasks[task_count].id = task_count;
    tasks[task_count].task_function = func;
    tasks[task_count].delay = delay;
    tasks[task_count].period = period;
    tasks[task_count].is_periodic = is_periodic;
    tasks[task_count].next_run_time = get_system_time() + delay;
    tasks[task_count].active = true;
    task_count++;
}

void schedule_task() {
    uint32_t now = get_system_time();
    for (int i = 0; i < task_count; i++) {
        if (!tasks[i].active) continue;
        if (now >= tasks[i].next_run_time) {
            tasks[i].task_function();
            if (tasks[i].is_periodic && tasks[i].period > 0) {
                tasks[i].next_run_time += tasks[i].period;
            } else {
                tasks[i].active = false; // one-shot
            }
        }
    }
}

// Example task functions
void task1() { printf("Task 1 executed at %u ms\n", get_system_time()); }
void task2() { printf("Task 2 executed at %u ms\n", get_system_time()); }

int main() {
    add_task(task1, 1000, 2000, true);  // periodic every 2s after 1s delay
    add_task(task2, 500, 0, false);     // one-shot after 0.5s

    while (1) {
        schedule_task();
        struct timespec ts = {0, 100 * 1000000}; // sleep 100ms
        nanosleep(&ts, NULL);
    }
    return 0;
}