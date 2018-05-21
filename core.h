

#ifndef PROGETTO2018_CORE_H
#define PROGETTO2018_CORE_H

#include "strutture_dati.h"

int Casuale(int max);
void *start_core (void *par);
void start_task (long int *clock, Task* task, char type);
void print_task(char *output, int n_core, int long clock, int task_id, STATE stato);


#endif //PROGETTO2018_CORE_H
