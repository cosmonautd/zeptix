#include "roundrobin.h"

void init_scheduler(struct process* list, int size) {

    if(size > 0)
        list[0].running = 1;
}

int next_task(struct process* list, int size) {

    int i;
    for(i=0; i < size; i++)
        if(list[i].running) break;
        
    list[i].running = 0;
    if(i++==size)
        i = 0;
    list[i].running = 1;
    
    return i;
}
