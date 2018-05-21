

#ifndef PROGETTO2018_STRUTTURE_DATI_H
#define PROGETTO2018_STRUTTURE_DATI_H

#include <stdlib.h>
#include <stdio.h>

//Possibili stati di un processo
typedef enum {NEW, READY, RUNNING, BLOCKED, EXIT} STATE;

typedef struct Istruzione {
    int type_flag;
    int length;
}Istruzione;

typedef struct Task {
    int id;
    Istruzione *pc;
    long int arrival_time;
    Istruzione *instr_list;
    int n_istruzioni;
    int task_length;
    STATE stato;
    int ended;
}Task;



typedef  struct Parametri {
    int n_core;
    char *output;
    int n_task;
    int queue;
    Task** tasks;
    Istruzione** instrunctions;
    char type;
}Parametri;


int parse (char *file_input, Task **tasks, Istruzione **instructions);

#endif //PROGETTO2018_STRUTTURE_DATI_H
