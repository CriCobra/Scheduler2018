#include "scheduler.h"

FILE *o_file;

void start_sched(Task *tasks, Istruzione *instructions, int n_task, char *f_output, char type) {
    o_file = fopen(f_output, "w");
    if (o_file==NULL) {
        printf ("Impossibile aprire il file di output \n");
        return;
    }

    Task* task = NULL;
    Istruzione* instr = NULL;

    task = tasks;
    instr = instructions;

    //creo tid thread
    pthread_t core_0;
    pthread_t core_1;

    //creo struct Parametri per i due core (thread)
    struct Parametri p_0;
    struct Parametri p_1;


   //riempio con i parametri e faccio partire i due core
    fill_param(&p_0,0,f_output,n_task, &task, &instr, type);
    fill_param(&p_0,1,f_output,n_task, &task, &instr, type);




    /*int pthread_create(pthread_t*tidp,pthread_attr_t*attr, void  *(*start_rtn)(void*),void *arg);
		pthread_create()crea un nuovo thread
        Parametri della funzione
        tipd: tid del thread creato
        attr: per impostare gli attributi del thread (NULL di default)
        start_rtn: funzione eseguita dal thread creato
        arg: argomento di ingresso per start_rtn    */
    if (pthread_create(&core_0, NULL, &start_core, &p_0) != 0) {
        printf("Errore creazione core 0\n");
        return;
    }

    if (pthread_create(&core_1, NULL, &start_core, &p_1) != 0) {
        printf("Errore creazione core 1\n");
        return;
    }


    //pthread_join(pthread_t th, void **thread_return);
    //Sospende l’esecuzione del thread corrente fino alla fine del thread th.
    //aspetto che i core finiscano
    pthread_join(core_0, NULL);
    pthread_join(core_1, NULL);



    fclose(o_file);
    //libero la memoria allocata
    free(task);
    free(instr);
    

}



void fill_param(Parametri *p, int n_core, char *f_output, int n_task, Task** task, Istruzione** instrunction, char type) {
    p->n_core = n_core;
    p->output = f_output;
    p->n_task = n_task;
    p->queue = n_task;
    p->tasks = task;
    p->instrunctions = instrunction;
    p->type = type;
}


