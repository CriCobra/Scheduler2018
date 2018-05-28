#include <getopt.h>
#include "strutture_dati.h"
#include "scheduler.h"



char *outputPreemption;
char *outputNoPreemption;
char *input;
const char* program_name;



int main(int argc, char *argv[]) {
   Task *tasks = NULL;
    Istruzione *instructions = NULL;
     parametri(argc, argv);
    int n_task = parse(input, &tasks, &instructions);



    //The pid_t data type represents process IDs
     pid_t scheduler_preemptive, scheduler_notpreemptive;

       if ((scheduler_preemptive = fork()) != 0) {
           if ((scheduler_notpreemptive = fork()) != 0) {
               //sono nel padre
               //aspetto che gli scheduler finiscano
               wait(&scheduler_preemptive);
               wait(&scheduler_notpreemptive);

               return 0;

           } else  {
               //sono nel not preemptive
              start_sched(tasks, instructions, n_task, outputNoPreemption, 'N');



           }


       } else  {

           //sono nel preemptive

       //   start_sched(tasks, instructions, n_task, outputPreemption, 'P');




       }








    return 0;

}


void print_help(FILE* stream, int exit_code)
{
    fprintf(stream, "Help options\n");
    fprintf(stream,
            " -op   --output-preemption: il file di output con i risultati dello scheduler con preemption.\n"
                    " -on   --output-no-preemption: il file di output con i risultati dello scheduler senza preemption.\n"
                    " -i    --input: il file di input contenente la lista dei job.\n"
                    " -h    --help             Display this help information.\n");
    exit(exit_code);
}


void parametri (int argc, char* const argv[]) {

    int next_option;

    const char* const short_options = "o:i:h";

    const struct option long_options[] = {

            { "output-preemption", 	  1, NULL, 'p' },
            { "output-no-preemption", 1, NULL, 'n' },
            { "input",   		      1, NULL, 'i' },
            { "help",				  0, NULL, 'h' },
            { NULL, 				  0, NULL,  0  }
    };


//Per specificare le opzioni lunghe disponibili, puoi costruire un array di elementi struct option.
//Ogni elemento corrisponde a una opzione lunga ed ha quattro campi. In circostanze normali, il primo campo è il
//nome dell opzione lunga (come una stringa di caratteri, senza i due trattini); il secondo è 1 se l opzione
//prende un argomento o 0 altrimenti; Il terzo è NULL; e il quarto è un carattere costante che specifica
//l opzione breve sinonima per quella lunga. L ultimo elemento dell'array dovrebbe essere tutto zeri.

//To accept GNU-style long options as well as single-character options, use getopt_long instead of getopt.
//int getopt_long (int argc, char *const *argv, const char *shortopts, const struct option *longopts, int *indexptr)
//When getopt_long encounters a short option, it does the same thing that getopt would do: it returns the character
//code for the option, and stores the option’s argument (if it has one) in optarg.

    program_name = argv[0];

    if (argc != 7) {
        fprintf(stderr, "Argomenti errati\n");
        exit(-1);
    }

    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch(next_option)
        {
            case 'o':
                if (optarg[0] == 'p')
                {
                    outputPreemption = &optarg[2];
                }

                else if (optarg[0] == 'n')
                {
                    outputNoPreemption = &optarg[2];
                }
                else
                {
                    print_help(stderr, 1);
                }

                break;


            case 'i':
                input = optarg;

                if(access(input, F_OK) != -1 )
                {
                    //Il file di input inserito è valido
                }
                else
                {
                    printf("Il file di input inserito non è valido! \n");
                    print_help(stderr, 1);
                }
                break;

            case 'h':
                print_help(stdout, 0);
                break;

            case '?': //getopt_long restituisce ? quando inseriamo un'opzione non valida.
                fprintf(stderr, "E'stata inserita un'opzione non valida\n");
                print_help(stderr, 1);
                break;

            case -1:
                break;

            default:
                print_help(stderr, 1);
//The C library function void abort(void) abort the program execution and comes out directly from the place of the call.
                abort();
        }

    } while(next_option != -1);

}












