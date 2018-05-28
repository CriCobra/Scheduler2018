#include "strutture_dati.h"

int parse (char *file_input, Task **tasks, Istruzione **instructions) {
    FILE *i_file;
    i_file = fopen(file_input, "r");

   if (i_file == NULL) {
        printf ("Impossibile aprire il file \n");
        return 0;
    }

    int n_instr = 0;
    int n_task = 0;
    char c;

    //conto task e instr per allocare strutture
    while (!feof(i_file)) {
        fscanf(i_file, "%c", &c);
          switch (c) {
              case 't': n_task++;
                  break;
              case 'i': n_instr++;
                  break;
              default: ;
          }
        }



    Task *task = (Task*)malloc(n_task* sizeof(Task));
    Istruzione *istruzione = (Istruzione*)malloc(n_instr*sizeof(Istruzione));


    //riporto il puntatore del file all'inizio
    rewind(i_file);

   n_instr=0;
    n_task=0;



    while (!feof(i_file)) {
        fscanf(i_file, "%c", &c);
        switch (c) {
            case 't':
                if (n_task!= 0) {
                    task[n_task-1].instr_list =  &istruzione[n_instr-task[n_task-1].n_istruzioni];
                    task[n_task-1].pc = task[n_task-1].instr_list;
                }
                fscanf(i_file, ",%d,%li", &task[n_task].id, &task[n_task].arrival_time);
                task[n_task].stato = NEW;
                n_task++;
                break;
            case 'i': fscanf(i_file, ",%d,%d", &istruzione[n_instr].type_flag, &istruzione[n_instr].length);
                task[n_task-1].n_istruzioni++;
                task[n_task-1].task_length += istruzione->length;
                n_instr++;
                break;
            default:;

        }

    }


    fclose(i_file);


    *tasks = task;
    *instructions = istruzione;
    return n_task-1;

}
