#include "tasks.h"
#include <avr/io.h>
#include <util/delay.h>

int i = 0;

void task0(void){
    while(1) {
        asm volatile ("nop");
    }
}

void task1(void){
    DDRD|= (1<<PORTD5);
    while(1) {
        PORTD^=(1<<PORTD5);
        _delay_ms(100);
    }
}

void task2(void){
    DDRD|= (1<<PORTD6);
    while(1) {
        PORTD^=(1<<PORTD6);
        _delay_ms(100);
    }
}

void task3(void){
    DDRB |= (1 << DDB1);
    OCR1A = 0x01FF; // set PWM for 50% duty cycle @ 10bit
    TCCR1A |= (1 << COM1A1); // set none-inverting mode
    TCCR1A |= (1 << WGM11) | (1 << WGM10); // set 10bit phase corrected PWM Mode
    TCCR1B |= (1 << CS11); // set prescaler to 8 and starts PWM

    while(1) {
        while(i < 256) {
            OCR1A = i;
            _delay_ms(3);
            i++;
        }
        
        if( i >= 256){
            //i = 0;
            while(i > 0){
            OCR1A = i;
            _delay_ms(3);
            i--;
            }  
        }
        
    }
}
