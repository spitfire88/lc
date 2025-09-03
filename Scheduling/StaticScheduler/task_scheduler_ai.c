#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

int current_time = 0;

int get_time() {
    return current_time;
}

struct Task {
    int id;
    int period;
    int delay;
    bool is_periodic;
    int schedule_time;
    void (*task_func)(void);
};
typedef struct Task task_t;

#define MAX_TASKS 10

task_t task_array[MAX_TASKS];
int next_task_id = 0;

void init_scheduler() {
    return;
}

void add_task(void(*task_func)(void), int delay, int period, bool is_periodic) {
    if(next_task_id >= MAX_TASKS) {
        perror("MAX_TASKS reached, cannot add any more tasks");
        return;
    }
    task_array[next_task_id].delay = delay;
    task_array[next_task_id].period = period;
    task_array[next_task_id].is_periodic = is_periodic;
    task_array[next_task_id].task_func = task_func;
    task_array[next_task_id].schedule_time = get_time() + delay;
    next_task_id++;
    return;
}

void schedule() {
    for(int i = 0; i < next_task_id; i++) {
        if(task_array[i].schedule_time <= get_time()) {
            task_array[i].task_func();
            if(task_array[i].is_periodic) {
                task_array[i].schedule_time = get_time() + task_array[i].period;
            } else {
                task_array[i].schedule_time = INT32_MAX;
            }
        }
    }
    current_time += 100;
    return;
}

void task1() {
    printf("[%d] toggle led periodically\n", get_time());
}

void task2() {
    printf("[%d] turn engine on\n", get_time());
}

void task3() {
    printf("[%d] wipers on\n", get_time());
}

void task4() {
    printf("[%d] turn engine off\n", get_time());
    sleep(1);
    exit(1);
}

int main() {
    init_scheduler();
    add_task(task1, 100, 100, true);
    add_task(task2, 0, 0, false);
    bool flag = true;
    
    for(;;) {
        schedule();
        if(flag == true) {
            add_task(task3, 500, 400, true);
            flag = false;
        }
        sleep(2);
        if(get_time() == 1100) {
            add_task(task4, 0, 0, false);
        }
    }

    return 0;
}