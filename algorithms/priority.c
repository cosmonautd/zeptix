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

    list[0].priority = 0;
    list[1].priority = 10;
    list[2].priority = 150;
    list[3].priority = 512;

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

    for(i=1; i < size; i++) {
        list[i].stack_pointer-=35;
    }
}

int next_task(struct process* list, int size) {

    /*Variáveis que armazenarão o valor da maior prioridade
       encontrada entre os processos e o índice do processo
       com com maior prioridade */
    int greatest_priority = 0;
    int greatest_priority_index = 0;

    /*Busca pelo processo que estava sendo executado logo
    antes da interrupção*/
    int i;
    for(i=0; i < size; i++)
        if(list[i].running == 1) break;
    int currently_running = i;
    if(currently_running < size)
        list[currently_running].running = 0;

    /*Busca pelo processo com maior prioridade dentro da
    lista de processos. Se houver mais de um processo com
    o mesmo nível de prioridade, eles serão executados de
    forma circular.*/
    for(i=0; i < size; i++) {
        int j = (i + currently_running + 1) % size;
        if(list[j].priority > greatest_priority) {
            greatest_priority = list[j].priority;
            greatest_priority_index = j;
        }
    }
    
    /*Implementação da técnica de envelhecimento*/
    for(i=1; i < size; i++)
        if(list[i].priority < greatest_priority)
            list[i].priority++;

    /*Declara o próximo processo a ser executado e retorna
    seu índice*/
    list[greatest_priority_index].running = 1;
    return greatest_priority_index;
}
