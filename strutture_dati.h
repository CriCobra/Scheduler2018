

#ifndef PROGETTO2018_STRUTTURE_DATI_H
#define PROGETTO2018_STRUTTURE_DATI_H


//Possibili stati di un processo
typedef enum {NEW, READY, RUNNING, BLOCKED, EXIT} STATE;

typedef struct Istruzione {
    int type_flag;
    int length;
}Istruzione;

typedef struct Task {
    int id;
    Istruzione *pc;
    int arrival_time;
    Istruzione *instr_list;
    int n_istruzioni;
    STATE stato;
}Task;

typedef  struct Parametri{
    int n_core;
    int n_job;
    int job_rimanenti;
    char* file_out;
    Task** tasks;
    Istruzione** istruzioni;
}Parametri;


int parse (char *file_input, Task **tasks, Istruzione **instructions);

#endif //PROGETTO2018_STRUTTURE_DATI_H
