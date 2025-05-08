#ifndef CONFIG_H
#define CONFIG_H

#define MAX_PROCESS 10  // Max number of process constraint
#define TIME_QUANTUM 4 // RR

typedef struct Process{
    int PID;
    int arrival_time;
    int CPU_burst_time;
    int Priority;
    int turnaround_time;   // Total time from arrival to completion
    int waiting_time;      // Time spent waiting in ready queue
} Process;

typedef struct Node{
    Process* process;
    struct Node * next;
    struct Node * before;
    int in_queue;
} Node;

typedef struct Queue{
    int size;
    Node * head;
    Node * tail;
} Queue;

#endif