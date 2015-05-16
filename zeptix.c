#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "modules/context.h"
#include "algorithms/roundrobin.h"

#define TOTAL_TASKS 3

struct process processes[TOTAL_TASKS];

int current_task = 0;

ISR(TIMER0_OVF_vect, __attribute__((naked))) {

    DDRB|= (1<<PORTB0);
    PORTB^=(1<<PORTB0);

    CONTEXT_BACKUP();
    SP_BACKUP(processes[current_task].stack_pointer);
    
    current_task = next_task(processes, TOTAL_TASKS);

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
    for(i=1; i < TOTAL_TASKS; i++) {
        processes[i].stack_size = 100;
        processes[i].running = 0;
    }
    
    processes[0].priority = 0;
    processes[1].priority = 5;
    processes[2].priority = 5;
    
    processes[0].task_pointer = task0;
    processes[1].task_pointer = task1;
    processes[2].task_pointer = task2;

    processes[0].stack_pointer = (int) RAMEND-50;

    for(i=1; i < TOTAL_TASKS; i++)
        processes[i].stack_pointer = processes[i-1].stack_pointer - processes[i-1].stack_size;

    char *px;
    for(i=0; i < TOTAL_TASKS; i++) {
        px = (char *) processes[i].stack_pointer;
        *px = (char) ((int) processes[i].task_pointer);
        px = (char *) processes[i].stack_pointer-1;
        *px = (char) (((int) processes[i].task_pointer) >> 8);
    }

    for(i=1; i < TOTAL_TASKS; i++) {
        processes[i].stack_pointer-=35;
    }

    TIMSK = (1 << TOIE0);
    TCCR0 = (1 << CS02);
    TCNT0 = 0;

    sei();
}

main(void) {

    init();
    init_scheduler(processes, TOTAL_TASKS);
    task0();
}
