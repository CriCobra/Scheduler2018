#include "core.h"


#ifndef PROGETTO2018_SCHEDULER_H
#define PROGETTO2018_SCHEDULER_H


void fill_param(Parametri *p, int n_core, int n_task, int *queue, Task** task, char type);
void start_sched(Task *tasks, Istruzione *instructions, int n_task, char *f_output, char type);




#endif //PROGETTO2018_SCHEDULER_H
