#ifndef QUEUE_H
#define QUEUE_H

#include "config.h"

Queue* create_queue(int number_of_process, Node** node_array);
Node* create_node(Process* process);
void insert_node_queue(Queue * q, Node* node);
Node* dequeue(Queue* q);
Node* remove_node_queue(Queue * q, int location);
void print_queue(Queue* q);
void clean_queue(Queue* q);
void update_queue(Node** node_array, Queue* q, int time_step, int num_process);
void print_node_array(Node** node_array, int num_process);
#endif