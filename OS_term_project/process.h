#ifndef PROCESS_H
#define PROCESS_H
#include "config.h"
#define MAX_PROCESS 10  // Max number of process constraint
Process* create_one_process(int pid);
Node** create_processes(int num_process);
void clean_all(Node** node_array, int num_process);

#endif