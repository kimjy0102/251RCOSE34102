#ifndef QUEUE_H
#define QUEUE_H
#include "config.h"

Process* create_one_process(int pid);
Node** create_processes(int num_process);
void clean_all(Node** node_array, int num_process);

#endif