#include "core.h"

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


//Type N: notpreemptive First Come First Served (FCFS), P:preemptive Shortest Remaining Time First (SRTF)
void *start_core(void *par) {
    long int ck = 0;
    Parametri *p = (Parametri *) par;
    Task *task = p->tasks[0];
    while (p->queue != 0) {
        for (int i = 0; i < p->n_task; i++) {
            if (p->type == 'P')
                sort_task(&task, p->n_task, i);
            //blocco il mutex e analizzo gli stati
            pthread_mutex_lock(&task_mutex);
            switch (task[i].stato) {
                case NEW:
                    if (ck >= task[i].arrival_time) {   //come da specifiche
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);
                        task[i].stato = READY;
                        print_task(p->output, p->n_core, ck,  task[i].id, task[i].stato);
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);
                        start_task(&ck, &task[i]);
                        //controllo se il task è finito
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, ck,  task[i].id, task[i].stato);

                    }

                    break;
                case READY:
                    if (ck >= task[i].arrival_time) {
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);
                        start_task(&ck, &task[i]);
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);
                    }

                    break;

                case BLOCKED:
                    if (ck >= task[i].arrival_time) {
                        task[i].stato = RUNNING;
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);
                        start_task(&ck, &task[i]);
                        if (task[i].stato == EXIT) {
                            p->queue -= 1;  //lo tolgo dalla coda
                            task[i].ended = 1;  //aggiorno finito
                        }
                        print_task(p->output, p->n_core, ck, task[i].id, task[i].stato);

                    }
                    break;
                default:
                    break;


            }
            //quando ho finito le operazioni del task sblocco il mutex
            pthread_mutex_unlock(&task_mutex);
            //se il task non è finito aumento lo stesso il clock
            if (task[i].ended == 0) {
                ck++;
            }
        }
    }

//distruggo i mutex
        pthread_mutex_destroy(&task_mutex);
        pthread_mutex_destroy(&output_mutex);
        return NULL;


    }







void print_task(char *output, int n_core,long int ck, int task_id, STATE stato) {
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
    fprintf(o_file, "core%d, %li, %d, %s\n", n_core, ck, task_id, state);
    pthread_mutex_unlock(&output_mutex);   //sgancio il mutex quando ho finito

}


void start_task (long int *clock, Task* task) {
    if (task->n_istruzioni != 0) { //se ci sono ancora istruzioni da eseguire
        Istruzione* in = task->pc;
                if (in->type_flag == 0) {
                    *clock += in->length;
                    task->pc = task->instr_list++;
                    task->n_istruzioni--;
                    task->task_length -= in->length;
                } else if (in->type_flag == 1) {
                    task->stato = BLOCKED;
                    task->arrival_time = Casuale(in->length) + *clock;
                    in->type_flag = 0;
                }
    } else {
        task->stato = EXIT;
        return;
    }


}



void sort_task(Task** task, int n_task, int i) {
    Task *tasks = task[0];
    Task temp;
    int posizione = 0;
    int length = tasks[0].task_length;
    for (int j=1; j< n_task; j++) {
        if ((tasks[j].stato = READY)|| (tasks[j].stato = NEW)) {
            if (tasks[j].task_length < length) {
                posizione = j;
                length = tasks[j].task_length;
            }
        }
    }
        switch_task(&tasks[i], &temp);
        switch_task(&tasks[posizione], &tasks[i]);
        switch_task(&temp, &tasks[posizione]);
    }




void switch_task(Task* task1, Task* task2) {
    task1->task_length = task2->task_length;
    task1->instr_list = task2->instr_list;
    task1->n_istruzioni = task2->n_istruzioni;
    task1->arrival_time = task2->arrival_time;
    task1->id = task2->id;
    task1->pc = task2->pc;
    task1->stato = task2->stato;

}















