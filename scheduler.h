#include <stdlib.h>
#include <stdio.h>

#ifndef PROGETTO2018_SCHEDULER_H
#define PROGETTO2018_SCHEDULER_H


void fill_param(Parametri *p, int n_core, char *f_output, int n_task, Task** task, Istruzione** instrunction, char type);
void start_sched(Task *tasks, Istruzione *instructions, int n_task, char *f_output, char type);
void switch_task(Task* task1, Task* task2);
void sort_task(Task** task, int n_task);



#endif //PROGETTO2018_SCHEDULER_H
