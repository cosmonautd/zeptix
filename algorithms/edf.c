#include <stdlib.h>
#include "edf.h"
#include "../tasks.h"

/* Função de inicialização do escalonador
 Preenchimento da PCB (Process Control Block) para cada processo
 */
void init_scheduler(struct process* list, int size, int ramend) {
    
    /* Para cada processo, inicializa o valor de tamanho da pilha alocada ao processo
     Indica que inicialmente, nenhum processo está sendo executado
     */
    int i;
    for(i=0; i < size; i++) {
        list[i].stack_size = 100;
        list[i].running = 0;
    }
    
    /* Associa a cada processo um conjunto de instruções armazenadas dentro de uma função
     Isso significa que o código associado ao processo 0 é definido na função task0
     O código associado ao processo 1 é definido na função task1 e assim por diante...
     */
    list[0].task_pointer = task0;
    list[1].task_pointer = task1;
    list[2].task_pointer = task2;
    list[3].task_pointer = task3;
    
    list[0].deadline = 0;
    list[1].deadline = 99;
    list[2].deadline = 494;
    list[3].deadline = 1495;

    /* O endereço da pilha do processo 0 é 100 bytes abaixo do último endereço na SRAM
     */
    list[0].stack_pointer = (int) ramend-100;
    
    /* Para cada processo subsequente, o endereço da pilha é definido
     */
    for(i=1; i < size; i++)
        list[i].stack_pointer = list[i-1].stack_pointer - list[i-1].stack_size;
    
    /* Quando os processos são executados pela primeira vez, é necessário que
     seus respectivos PCs (Program Counter) estejam adequadamente armazenados
     no topo da pilha do processo. Abaixo, essa medida é realizada.
     */
    char *px;
    for(i=1; i < size; i++) {
        px = (char *) list[i].stack_pointer;
        *px = (char) ((int) list[i].task_pointer);
        px = (char *) list[i].stack_pointer-1;
        *px = (char) (((int) list[i].task_pointer) >> 8);
    }
    
    /* O contexto no ATmega8 consiste em:
     32 registradores da CPU + 1 registrador de STATUS + 2 bytes do Program Counter
     Assim, 35 bytes são alocados na pilha de cada processo para armazenamento de seu contexto
     */
    for(i=0; i < size; i++) {
        list[i].stack_pointer-=35;
    }
}

/* Definição do algoritmo de escalonamento
 */
int next_task(struct process* list, int size) {
    
    int j, i = 0, closer_deadline = -1, aux_closer_deadline, deadline_last_process_executed, aux_last_process_executed;
    
    aux_closer_deadline = 1000000;
    
    // Subtrai o deadline em todos os processos e sinaliza os finalizados
    for (j = 0; j < size; j++){
        list[j].deadline--;
        if(list[j].deadline <= 0)
            list[j].finished = 1;
    }
    
    // Verifica qual processo está sendo executado anteriormente
    for (j = 0; j < size; j++){
        if(list[j].running){
            list[j].running = 0;
            aux_last_process_executed = j;
        }
    }
    
    // Variável i armazena quantos processos já foram finalizados
    for (j = 0; j < size; j++)
        if(list[j].finished == 1)
            i++;

    // Se existir somente um processo para ser executado, deve ser executado novamente
    if(i == size - 1)
        closer_deadline = aux_last_process_executed;
    else // Senão escolhe o proximo processo com o menor tempo para ser executado
        for (j = 0; j < size; j++){
            if (list[j].deadline > 0){
                if (list[j].deadline <= aux_closer_deadline && list[j].finished == 0 && j != aux_last_process_executed){
                    aux_closer_deadline = list[j].deadline;
                    closer_deadline = j;
                }
            }
        }   
        
    list[closer_deadline].running = 1;
    
    return closer_deadline;
}
