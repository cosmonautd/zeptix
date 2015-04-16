#define __SFR_OFFSET 0	
#include<avr/io.h>

.EQU    TOTAL_TASK,        7
.EQU    TASK_INDEX,        (RAMEND)
.EQU    SP_BACKUP_BASE,    (RAMEND-2)                   ; STACK POINTER BACKUP ARRAY BASE
.EQU    TASK1_STACK_BEGIN, (RAMEND-50)                  ; INITIAL STACK POINTER FOR TASK1
.EQU    TASK2_STACK_BEGIN, (RAMEND-350)                 ; INITIAL STACK POINTER FOR TASK2
.EQU    TASK3_STACK_BEGIN, (RAMEND-650)                 ; INITIAL STACK POINTER FOR TASK3
.EQU    TASK4_STACK_BEGIN, (RAMEND-950)                 ; INITIAL STACK POINTER FOR TASK4
.EQU    TASK5_STACK_BEGIN, (RAMEND-1250)                ; INITIAL STACK POINTER FOR TASK5
.EQU    TASK6_STACK_BEGIN, (RAMEND-1550)                ; INITIAL STACK POINTER FOR TASK6
.EQU    TASK7_STACK_BEGIN, (RAMEND-1850)                ; INITIAL STACK POINTER FOR TASK7

.section .text

; ROTINA PRINCIPAL

.global main
main:
    RCALL   INIT                                        ; CHAMAR SUBROTINA DE INICIALIZAÇÃO
    LDI     R16, 0xFF                                   ; SETAR TODOS OS BITS DE PORTD EM ALTO
    OUT     PORTD, R16
    NOP

LOOP:
    RJMP    LOOP                                        ; LOOP INFINITO


; SUBROTINAS

INIT:
    LDI     R16, 0xFF                                   ; CONFIGURAR PORTD COMO SAÍDA DIGITAL
    OUT     DDRD, R16

    ; CONFIGURAÇÃO DA INTERRUPÇÃO POR TIMER0
    ; TIMER_COUNT = DELAY*(CLOCK_FREQUENCY/PRESCALER) - 1
    ; ((2^(N_BITS)-TCNTO_INIT)*PRESCALER)/CLOCK_FREQUENCY = DELAY

    LDI     R16, (1<<TOIE0)                             ; HABILITAR INTERRUPÇÃO POR TIMER0
    OUT     TIMSK, R16
    LDI     R16, (1<<CS00) | (1<<CS02)                  ; SETAR PRESCALER PARA 1024
    OUT     TCCR0, R16
    LDI     R16, 0                                      ; SETAR CONTADOR DO TIMER0 PARA 0
    OUT     TCNT0, R16
    
    SEI                                                 ; HABILITAR INTERRUPÇÃO GLOBAL
    
    RET


; INTERRUPT SERVICE ROUTINES

.global TIMER0_OVF_vect                                 ; DEFINIÇÃO DA ISR PARA OVERFLOW DO TIMER0
TIMER0_OVF_vect:
    LDI     R24, 0xFF
    IN      R25, PORTD                                  ; REALIZAR LEITURA DE PORTD
    EOR     R24, R25
    OUT     PORTD, R24                                  ; INVERTER ESTADO DOS BITS DE PORTD
    RETI

.global __vector_default
__vector_default:
    RETI

.end
