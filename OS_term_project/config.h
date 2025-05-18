#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    CPU_IDLE,
    CPU_BUSY,
} STATUS;
extern int time_step;
extern int complete_process;

typedef struct Process{
    int PID;
    int arrival_time;
    int CPU_burst_time;
    int Priority;
    int turnaround_time;   // Total time from arrival to completion
    int waiting_time;      // Time spent waiting in ready queue
    // I/O
    int MAX_CPU_time;
    int* IO_burst_time;
    int* IO_request_time;
    int IO_num; 
    int i;              // index for IO burst & request array
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