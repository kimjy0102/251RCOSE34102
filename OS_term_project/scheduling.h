#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "Gantchart.h"
#include "config.h"
void FCFS(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart);
Node* FCFS_sub(Queue* q, Queue* waiting_queue, int time_step, Node* running, Gantchart* cpu_chart, Gantchart* io_chart);
void SJF_nonpreemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* chart, Gantchart* io_chart);
Node* SJF_nonpreemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart);
void Priority_nonpreemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart);
Node* Priority_nonpreemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart);
void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart);
Node* SJF_preemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart);
void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart);
Node* Priority_preemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart);
void Round_Robin(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart);
Node* Round_Robin_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart);

#endif