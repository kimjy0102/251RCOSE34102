#ifndef EVALUATION_H
#define EVALUATION_H
#include "config.h"

void print_turnaround_time(int num_process, Node** node_array);
void print_waiting_time(int num_process, Node** node_array);
void update_waiting_time(int num_process, Node** node_array);

#endif 