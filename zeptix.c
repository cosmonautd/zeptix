#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "modules/context.h"
#include "algorithms/roundrobin.h"

/* Definição de macros para inicialização e transmissão serial UART.
   baudrate: 2400, bytesize: 8, paridade: ímpar, stopbits: 2
*/
#define UART_INIT() UCSRB = 152; UCSRC = 142; UBRRL = 25; UBRRH = 0
#define UART_TX(data) while(!(UCSRA&(1<<UDRE))); UDR = data

/* Número de tarefas
*/
#define TOTAL_TASKS 4

/* Vetor de estruturas process
*/
struct process processes[TOTAL_TASKS];

/* Marcador da atual tarefa sendo executada
*/
int current_task = 0;

/* Definição da ISR para interrupção por overflow do TIMER0
*/
ISR(TIMER0_OVF_vect, __attribute__((naked))) {

    /* Muda o estado de um LED em PORTB0
    */
    DDRB|= (1<<PORTB0);
    PORTB^=(1<<PORTB0);

    /* Salva o contexto do processo na pilha do programa atual
       Guarda o valor de SP (Stack Pointer) na PCB (Process Control Block) do processo atual
    */
    CONTEXT_BACKUP();
    SP_BACKUP(processes[current_task].stack_pointer);
    
    /* Carrega o índice da próxima tarefa a ser executada
       Chama a função next_task(), definida no código do algoritmo de escalonamento utilizado
    */
    current_task = next_task(processes, TOTAL_TASKS);

    /* Carrega o valor de SP com o valor de SP do próximo processo
       Restaura o contexto do próximo processo
    */
    SP_RESTORE(processes[current_task].stack_pointer);
    CONTEXT_RESTORE();
    
    /* Transmite via serial o índice da próxima tarefa a ser executada
    */
    UART_TX(current_task);
    asm volatile ("nop");

    /* Retorna da interrupção
       Inicia a execução do próximo processo
    */
    asm volatile ("reti");
}

/* Definição da função init(), contendo o código de inicialização
*/
void init(void) {

    /* Inicializa UART
    */
    UART_INIT();
    
    /* Inicializa o escalonador de processos
       Preenche a PCB para todos os processos
    */
    init_scheduler(processes, TOTAL_TASKS, RAMEND);

    /* Configura a interrupção por overflow do TIMER0
       Seta bit para habilitação dessa interrupção
       Configura prescaler
       Inicializa contador do TIMER0
    */
    TIMSK = (1 << TOIE0);
    TCCR0 = (1 << CS02);
    TCNT0 = 0;
    
    /* Transmite o valor 255, sinalizando o término da inicialização
    */
    UART_TX(255);
    
    /* Habilita interrupção global
    */
    sei();
}

/* Definição da função principal
*/
main(void) {

    /* Inicialização
    */
    init();
    task0();
}
