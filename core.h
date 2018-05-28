

#ifndef PROGETTO2018_CORE_H
#define PROGETTO2018_CORE_H

#include "strutture_dati.h"
#include <pthread.h>

int Casuale(int max);
void *start_core(void *par);
void start_task (int long *clock, Task* task);
void print_task(int n_core, long int ck, int task_id, STATE stato);
void switch_task(Task* task1, Task* task2);
void sort_task(Task** task, int n_task, int i);


#endif //PROGETTO2018_CORE_H
