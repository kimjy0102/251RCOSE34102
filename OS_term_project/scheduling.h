#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "config.h"
void FCFS(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
//void SJF_nonpreemptive(Queue* ready_queue, Node** pcb_array, int num_process);
//void SJF_preemptive(Queue* ready_queue, Node** pcb_array, int num_process);
//void Priority_nonpreemptive(Queue* ready_queue, Node** pcb_array, int num_process);
//void Priority_preemptive(Queue* ready_queue, Node** pcb_array, int num_process);
//void RR(Queue* ready_queue, Node** pcb_array, int num_process);
Node* FCFS_sub(Queue* q, int time_step, Node* running, STATUS* cpu_status, Gantchart* chart);






#endif