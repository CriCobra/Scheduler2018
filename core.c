#include "core.h"
#include "strutture_dati.h"
#include <pthread.h>
#include <_stdio.h>
#include <stdlib.h>


FILE* o_file = NULL;

//Il mutex è un tipo definito "ad hoc" per gestire la mutua
//esclusione quindi il valore iniziale può essergli assegnato
//anche in modo statico mediante questa macro.

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;   //mutex per scrivere su file
pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;     // mutex per analizzare un task




int Casuale(int max) {
    int numero;
    numero=rand()%(max+1);
    return numero;
}



void *start_core (void *par) {
    long int clock = 0;
    Parametri *p = (Parametri*) par;
    Task* task = p->tasks[0];

    while (p->queue != 0) {
        for (int i = 0; i < p->n_task; i++) {
             //blocco il mutex e analizzo gli stati
             pthread_mutex_lock(&task_mutex);
             switch (task[i].stato) {
                case NEW:
                    if (clock >= task[i].arrival_time) {   //come da specifiche
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                        task[i].stato = READY;
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i], p->type);
                        //controllo se il job è finito
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);

                    }

                    break;
                case READY:
                    if (clock >= task[i].arrival_time) {
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i], p->type);
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                    }

                    break;

                case BLOCKED:
                    if (clock >= task[i].arrival_time) {
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i], p->type);
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, clock, task[i].id, task[i].stato);

                    }
                    break;
                default:
                    break;



            }
            //quando ho finito le operazioni del task sblocco il mutex
            pthread_mutex_unlock(&task_mutex);
            //se il task non è finito aumento lo stesso il clock
            if (task[i].ended == 0) {
                clock++;
            }
        }
}
//distruggo i mutex
pthread_mutex_destroy(&task_mutex);
pthread_mutex_destroy(&output_mutex);
return NULL;


}





void print_task(char *output, int n_core, int long clock, int task_id, STATE stato) {
    const char *state = NULL;
    switch (stato) {
        case NEW:
            state = "new";
            break;
        case READY:
            state = "ready";
            break;
        case RUNNING:
            state = "running";
            break;
        case BLOCKED:
            state = "blocked";
            break;
        case EXIT:
            state = "exit";
            break;
    }

    pthread_mutex_lock(&output_mutex);  //acquisisco il mutex per scrivere su file
    fprintf(o_file, "core%d, %li, %d, %s\n", n_core, clock, task_id, state);
    pthread_mutex_unlock(&output_mutex);   //sgancio il mutex quando ho finito

}


void start_task (long int *clock, Task* task, char type) {
   //Type P:preemptive, N:notpreemptive
    Istruzione* in = task->pc;
    switch (type) {
        case 'N':

            if (in->type_flag == 0) {
                *clock += in->length;
                task->pc = task->instr_list++;
            } else if (in->type_flag == 1) {
                task->stato = BLOCKED;
                task->arrival_time = Casuale(in->length) + *clock;
                in->type_flag = 0;
            }
                break;
        case 'P': //calcolare somma istruzioni SJF

            break;










            }


    }












