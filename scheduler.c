#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include "strutture_dati.h"

FILE *o_file;

void Inserimento_parametri(int n_core, char* out_file, Task** task, Istruzione** instr, Parametri *parametro, int n_task);

void start_sched(Task *tasks, Istruzione *instructions, int n_task, char *f_output) {
    o_file = fopen(f_output, "w");
    if (o_file==NULL) {
        printf ("Impossibile aprire il file di output \n");
        return;
    }

    Task* task = NULL;
    Istruzione* instr = NULL;

    task = tasks;
    instr = instructions;


    pthread_t core_0;
    pthread_t core_1;

    int err;

    Parametri parametri_0;
    Parametri parametri_1;

    Inserimento_parametri(0, o_file, &task, &instr, &parametri_0, n_task);
    Inserimento_parametri(1, o_file, &task, &instr, &parametri_1, n_task);

    err = pthread_create(&(core_0), NULL, core, &parametri_0);
    if(err != 0){
        printf("Non posso creare il core0");
        exit(EX_OSERR);
    }
    err = pthread_create(&(core_1), NULL, core, &parametri_1);
    if(err != 0){
        printf("Non posso creare il core1");
        exit(EX_OSERR);
    }

    pthread_join(core_0, NULL);
    pthread_join(core_1, NULL);

    fclose(o_file);
    return;

}

void Inserimento_parametri(int n_core, char* out_file, Task** task, Istruzione** instr, Parametri *parametro, int n_task){
    parametro->file_out = out_file;
    parametro->istruzioni = instr;
    parametro->n_core = n_core;
    parametro->tasks = task;
    parametro->n_job = n_task;
    parametro->job_rimanenti = n_task;
}









}




