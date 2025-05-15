#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "Gantchart.h"
#include "config.h"
void FCFS(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* FCFS_sub(Queue* q, int time_step, Node* running, STATUS* cpu_status, Gantchart* chart);
void SJF_nonpreemptive(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* SJF_nonpreemptive_sub(Queue* ready_queue, int time_step, Node* running_process, STATUS* cpu_status, Gantchart* chart);
void Priority_nonpreemptive(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* Priority_nonpreemptive_sub(Queue* ready_queue, int time_step, Node* running_process, STATUS* cpu_status, Gantchart* chart);
void SJF_preemptive(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* SJF_preemptive_sub(Queue* ready_queue, int time_step, Node* running_process, STATUS* cpu_status, Gantchart* chart);
void Priority_preemptive(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* Priority_preemptive_sub(Queue* ready_queue, int time_step, Node* running_process, STATUS* cpu_status, Gantchart* chart);
void Round_Robin(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart);
Node* Round_Robin_sub(Queue* ready_queue, int time_step, Node* running_process, STATUS* cpu_status, Gantchart* chart);

#endif