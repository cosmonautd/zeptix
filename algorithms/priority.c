#include "priority.h"
#include "../tasks.h"

void init_scheduler(struct process* list, int size, int ramend) {

    int i;
    for(i=0; i < size; i++) {
        list[i].stack_size = 100;
        list[i].running = 0;
    }

    list[0].task_pointer = task0;
    list[1].task_pointer = task1;
    list[2].task_pointer = task2;
    list[3].task_pointer = task3;

    list[0].stack_pointer = (int) ramend-100;

    for(i=1; i < size; i++)
        list[i].stack_pointer = list[i-1].stack_pointer - list[i-1].stack_size;

    char *px;
    for(i=0; i < size; i++) {
        px = (char *) list[i].stack_pointer;
        *px = (char) ((int) list[i].task_pointer);
        px = (char *) list[i].stack_pointer-1;
        *px = (char) (((int) list[i].task_pointer) >> 8);
    }

    for(i=0; i < size; i++) {
        list[i].stack_pointer-=35;
    }
}

int next_task(struct process* list, int size) {

    int i = 3;
    list[i].running = 1;

    return i;
}
