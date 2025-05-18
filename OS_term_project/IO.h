#ifndef IO_H

#define IO_H
#include "config.h"

int create_num_IO(int CPU_burst_time);
int* create_burst_time(int num);
int* create_request_time(int num, int CPU_burst_time);
Queue* create_waiting_queue(void);
void running_to_waiting_queue(Queue* waiting_queue, Node* running_process);
void waiting_to_ready_queue(Queue* waiting_queue, Queue* ready_queue);
void update_io_queue(Queue* waiting_queue, Queue* ready_queue);
void print_io_queue(Queue* q);
#endif