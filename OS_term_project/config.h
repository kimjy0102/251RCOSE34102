#ifndef CONFIG_H

#define CONFIG_H
#define MAX_PROCESS 20
#define TIME_QUANTUM 4
#include<string.h>

Queue* create_queue(int number_of_process, Node** node_array);
void enqueue(Queue * q, Node* node);
void dequeue(Queue * q, Node* node);

typedef struct Process{
    int PID;
    int arrival_time;
    int CPU_burst_time;
    int Priority;   
} Process;

typedef struct Node{
    Process* process;
    struct Node * next;
    struct Node * before;
} Node;

typedef struct Queue{
    int size;
    Node * head;
    Node * tail;
} Queue;
#endif