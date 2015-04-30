#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "context.h"

#define TOTAL_TASKS 3

struct Process {
   int  pid;
   int  stack_size;
   int  stack_pointer;
   void *task_pointer;
};  

struct Process processes[TOTAL_TASKS];

int current_task = 0;
/*int task_stack_size[TOTAL_TASKS];
int SP_backup_array[TOTAL_TASKS];*/

ISR(TIMER0_OVF_vect, __attribute__((naked))) {

    DDRB|= (1<<PORTB0);
    PORTB^=(1<<PORTB0);

    CONTEXT_BACKUP();
    /*SP_BACKUP(SP_backup_array[current_task]);*/
    SP_BACKUP(processes[current_task].stack_pointer);
    

    if(current_task++==TOTAL_TASKS)
        current_task = 0;

    /*SP_RESTORE(SP_backup_array[current_task]);*/
    SP_RESTORE(processes[current_task].stack_pointer);
    CONTEXT_RESTORE();

    asm volatile ("reti");
}

void task0(void){
    DDRD|= (1<<PORTD5);
    while(1) {
        PORTD^=(1<<PORTD5);
    }
}

void task1(void){
    DDRD|= (1<<PORTD6);
    while(1) {
        PORTD^=(1<<PORTD6);
    }
}

void task2(void){
    DDRD|= (1<<PORTD7);
    while(1) {
        PORTD^=(1<<PORTD7);
    }
}

void init(void) {

    int i;
    for(i=1; i < TOTAL_TASKS; i++){
        /*task_stack_size[i] = 100;*/
        processes[i].stack_size = 100;
    }

    /*void (*task_pointer[TOTAL_TASKS]);
    task_pointer[0] = task0;
    task_pointer[1] = task1;
    task_pointer[2] = task2;*/
    
    processes[0].task_pointer = task0;
    processes[1].task_pointer = task1;
    processes[2].task_pointer = task2;

    /*SP_backup_array[0] = (int) RAMEND-50;*/
    processes[0].stack_pointer = (int) RAMEND-50;

    for(i=1; i < TOTAL_TASKS; i++) {
        /*SP_backup_array[i] = SP_backup_array[i-1] - task_stack_size[i-1];*/
        processes[i].stack_pointer = processes[i-1].stack_pointer - processes[i-1].stack_size;
    }

    char *px;
    for(i=0; i < TOTAL_TASKS; i++) {
        /*px = (char *) SP_backup_array[i];
        *px = (char) ((int) task_pointer[i]);
        px = (char *) SP_backup_array[i]-1;
        *px = (char) (((int) task_pointer[i]) >> 8);*/
        px = (char *) processes[i].stack_pointer;
        *px = (char) ((int) processes[i].task_pointer);
        px = (char *) processes[i].stack_pointer-1;
        *px = (char) (((int) processes[i].task_pointer) >> 8);
    }

    for(i=1; i < TOTAL_TASKS; i++) {
        /*SP_backup_array[i]-=35;*/
        processes[i].stack_pointer-=35;
    }

    TIMSK = (1 << TOIE0);
    TCCR0 = (1 << CS00) | (1 << CS02);
    TCNT0 = 0;

    /*SP_RESTORE(SP_backup_array[current_task]);*/
    //SP_RESTORE(processes[current_task].stack_pointer);

    sei();
}

main(void) {

    init();
    task0();
}
