#include "core.h"

FILE* o_file = NULL;

//Il mutex è un tipo definito "ad hoc" per gestire la mutua
//esclusione quindi il valore iniziale può essergli assegnato
//anche in modo statico mediante questa macro.

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;   //mutex per scrivere su file
pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;     // mutex per analizzare un task
pthread_mutex_t sort_mutex = PTHREAD_MUTEX_INITIALIZER;



int Casuale(int max) {
    int numero;
    numero=rand()%(max+1);
    return numero;
}


//Type N: notpreemptive First Come First Served (FCFS), P:preemptive Shortest Remaining Time First (SRTF)
void *start_core(void *par) {
    long int clock = 0;
    Parametri *p = (Parametri *) par;
    Task *task = p->tasks[0];
    while (*p->queue > 0) {
        for (int i = 0; i < p->n_task; i++) {
            printf("%li\n", task[0].arrival_time);

            if ((i>0) && (task[0].stato == NEW))  //per far arrivare il clock all'arrival time minimo
                    i--;
            if ((p->type == 'N') && (i>0) && (task[i-1].stato==READY))
                i--;
            if (p->type == 'P') {
                pthread_mutex_lock(&sort_mutex);
                sort_task(&task, p->n_task, i);

                pthread_mutex_unlock(&sort_mutex);
            }

          //  printf("%d\n", i);
            //blocco il mutex e analizzo gli stati
            pthread_mutex_lock(&task_mutex);
            switch (task[i].stato) {
                case NEW:
                    if (clock >= task[i].arrival_time) {   //come da specifiche
                        print_task(p->n_core, clock, task[i].id, task[i].stato);
                        task[i].stato = READY;
                        print_task(p->n_core, clock,  task[i].id, task[i].stato);
                        task[i].stato = RUNNING;
                        print_task(p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i]);
                        //controllo se il task è finito
                        if (task[i].stato == EXIT) {
                            *p->queue -= 1;  //lo tolgo dalla coda
                            print_task(p->n_core, clock,  task[i].id, task[i].stato);
                        }
                        if (task[i].stato == BLOCKED)
                            print_task(p->n_core, clock,  task[i].id, task[i].stato);
                        if (task[i].stato == READY && p->type== 'P')
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);


                    }

                    break;
                case READY:
                    if (clock >= task[i].arrival_time) {
                        task[i].stato = RUNNING;
                        if (p->type == 'P')
                            print_task(p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i]);
                        if (task[i].stato == EXIT) {
                            *p->queue -= 1;  //lo tolgo dalla coda
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);
                        }
                        if (task[i].stato == BLOCKED)
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);
                        if (task[i].stato == READY && p->type== 'P')
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);
                    }

                    break;

                case BLOCKED:
                    if (clock >= task[i].arrival_time) {
                        task[i].stato = READY;
                        print_task( p->n_core, clock,  task[i].id, task[i].stato);
                        task[i].stato = RUNNING;
                        print_task( p->n_core, clock, task[i].id, task[i].stato);
                        start_task(&clock, &task[i]);
                        if (task[i].stato == EXIT) {
                            *p->queue -= 1;  //lo tolgo dalla coda
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);
                        }
                        if (task[i].stato == BLOCKED)
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);
                        if (task[i].stato == READY && p->type== 'P')
                            print_task( p->n_core, clock,  task[i].id, task[i].stato);

                    }
                    break;
                default:
                    break;


            }
            clock++;

            //quando ho finito le operazioni del task sblocco il mutex
            pthread_mutex_unlock(&task_mutex);

        }

    }

//distruggo i mutex
        pthread_mutex_destroy(&task_mutex);
        pthread_mutex_destroy(&output_mutex);
        return NULL;


    }







void print_task(int n_core,long int ck, int task_id, STATE stato) {
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


void start_task (int long *clock, Task* task) {
    if (task->n_istruzioni != 0) { //se ci sono ancora istruzioni da eseguire
        Istruzione* in = task->pc;
                if (in->type_flag == 0) {
                    *clock += in->length;
                    task->pc = task->instr_list++;
                    task->n_istruzioni--;
                    task->task_length -= in->length;
                    task->stato = READY;
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
    temp.task_length = 0;
    temp.instr_list = 0;
    temp.n_istruzioni = 0;
    temp.arrival_time = 0;
    temp.id = 0;
    temp.pc = 0;
    temp.stato = NEW;


    int posizione = 0;
    int length = tasks[0].task_length;
    for (int j=1; j< n_task; j++) {
        if ((tasks[j].stato == READY)|| (tasks[j].stato == NEW)) {
            if (tasks[j].task_length < length) {
                posizione = j;
                length = tasks[j].task_length;
            }
        }
    }
        switch_task(&temp, &tasks[i]);
        switch_task(&tasks[i], &tasks[posizione]);
        switch_task(&tasks[posizione], &temp);

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















