#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CONTEXT_BACKUP() \
            asm( \
                    "PUSH R0    \n\t" \
                    "PUSH R1    \n\t" \
                    "PUSH R2    \n\t" \
                    "PUSH R3    \n\t" \
                    "PUSH R4    \n\t" \
                    "PUSH R5    \n\t" \
                    "PUSH R6    \n\t" \
                    "PUSH R7    \n\t" \
                    "PUSH R8    \n\t" \
                    "PUSH R9    \n\t" \
                    "PUSH R10   \n\t" \
                    "PUSH R11   \n\t" \
                    "PUSH R12   \n\t" \
                    "PUSH R13   \n\t" \
                    "PUSH R14   \n\t" \
                    "PUSH R15   \n\t" \
                    "PUSH R16   \n\t" \
                    "PUSH R17   \n\t" \
                    "PUSH R18   \n\t" \
                    "PUSH R19   \n\t" \
                    "PUSH R20   \n\t" \
                    "PUSH R21   \n\t" \
                    "PUSH R22   \n\t" \
                    "PUSH R23   \n\t" \
                    "PUSH R24   \n\t" \
                    "PUSH R25   \n\t" \
                    "PUSH R26   \n\t" \
                    "PUSH R27   \n\t" \
                    "PUSH R28   \n\t" \
                    "PUSH R29   \n\t" \
                    "PUSH R30   \n\t" \
                    "PUSH R31   \n\t" \
                    "IN R0,0x3F \n\t" \
                    "PUSH R0    \n\t" \
                    );

#define SP_BACKUP(dest) (dest = SP)

#define CONTEXT_RESTORE() \
            asm( \
                    "POP R0      \n\t" \
                    "OUT 0x3F,R0 \n\t" \
                    "POP R31     \n\t" \
                    "POP R30     \n\t" \
                    "POP R29     \n\t" \
                    "POP R28     \n\t" \
                    "POP R27     \n\t" \
                    "POP R26     \n\t" \
                    "POP R25     \n\t" \
                    "POP R24     \n\t" \
                    "POP R23     \n\t" \
                    "POP R22     \n\t" \
                    "POP R21     \n\t" \
                    "POP R20     \n\t" \
                    "POP R19     \n\t" \
                    "POP R18     \n\t" \
                    "POP R17     \n\t" \
                    "POP R16     \n\t" \
                    "POP R15     \n\t" \
                    "POP R14     \n\t" \
                    "POP R13     \n\t" \
                    "POP R12     \n\t" \
                    "POP R11     \n\t" \
                    "POP R10     \n\t" \
                    "POP R9      \n\t" \
                    "POP R8      \n\t" \
                    "POP R7      \n\t" \
                    "POP R6      \n\t" \
                    "POP R5      \n\t" \
                    "POP R4      \n\t" \
                    "POP R3      \n\t" \
                    "POP R2      \n\t" \
                    "POP R1      \n\t" \
                    "POP R0      \n\t" \
                    );

#define SP_RESTORE(value) (SP = (int) (value))

#define TOTAL_TASKS 3
int current_task = 0;
int task_stack_size[TOTAL_TASKS];
int SP_backup_array[TOTAL_TASKS];

ISR(TIMER0_OVF_vect, __attribute__((naked))) {

    DDRB|= (1<<PORTB0);
    PORTB^=(1<<PORTB0);
    
    CONTEXT_BACKUP();
    SP_BACKUP(SP_backup_array[current_task]);
    
    if(current_task++==TOTAL_TASKS)
        current_task = 0;
    
    SP_RESTORE(SP_backup_array[current_task]);
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
    for(i=1; i < TOTAL_TASKS; i++)
        task_stack_size[i] = 100;
    
    void (*task_pointer[TOTAL_TASKS]);
    task_pointer[0] = task0;
    task_pointer[1] = task1;
    task_pointer[2] = task2;
    
    SP_backup_array[0] = (int) RAMEND-50;
    
    for(i=1; i < TOTAL_TASKS; i++)
        SP_backup_array[i] = SP_backup_array[i-1] - task_stack_size[i-1];
    
    char *px;
    for(i=0; i < TOTAL_TASKS; i++) {
        px = (char *) SP_backup_array[i];
        *px = (char) ((int) task_pointer[i]);
        px = (char *) SP_backup_array[i]-1;
        *px = (char) (((int) task_pointer[i]) >> 8);
    }
    
    for(i=1; i < TOTAL_TASKS; i++)
        SP_backup_array[i]-=35;
    
    TIMSK = (1 << TOIE0);
    TCCR0 = (1 << CS02);// | (1 << CS02);
    TCNT0 = 0;
    
    SP_RESTORE(SP_backup_array[current_task]);
    
    sei();
}

main(void) {
    
    init();
    task0();
}
