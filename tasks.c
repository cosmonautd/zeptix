#include "tasks.h"
#include <avr/io.h>
#include <util/delay.h>

void task0(void){
    while(1) {
        asm volatile ("nop");
    }
}

void task1(void){
    DDRD|= (1<<PORTD5);
    while(1) {
        PORTD^=(1<<PORTD5);
        _delay_ms(10);
    }
}

void task2(void){
    DDRD|= (1<<PORTD6);
    while(1) {
        PORTD^=(1<<PORTD6);
        _delay_ms(10);
    }
}

void task3(void){
    DDRD|= (1<<PORTD7);
    while(1) {
        PORTD^=(1<<PORTD7);
        _delay_ms(10);
    }
}
