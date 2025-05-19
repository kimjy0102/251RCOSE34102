#include <stdio.h>
#include "config.h"
#include <stdlib.h>
#include "queue.h"
int create_num_IO(int CPU_burst_time)
{
    // CPU burst time > number of execution of IO
    int max_io = CPU_burst_time / 3;
    int num = rand() % (max_io + 1); // num : 0 ~ max_io
    return num;
}

int* create_burst_time(int num)
{
    if ( num <= 0 )
        return NULL;
    int* burst_time = (int*)malloc(sizeof(int)*num);
    for (int i = 0 ; i < num ; i++)
    {
        int temp_time = rand() % 5 + 1; // 1 ~ 5
        burst_time[i] = temp_time;
    }
    return burst_time;
}

int* create_request_time(int num, int CPU_burst_time)
{
    if ( num <= 0)
        return NULL;
    int * request_time = (int*)malloc(sizeof(int)*(num));
    int slice = CPU_burst_time / num;
    for ( int i = 0; i < num ; i++)
    {
        int temp = i * slice;
        request_time[i] = temp + (rand() % slice + 1);
        if ( request_time[i] == 0)
            request_time[i]++;
        else if (request_time[i] == CPU_burst_time)
            request_time[i]--;
    }
    return request_time;
}

Queue* create_waiting_queue(void)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) 
    {
        printf("Queue Memory allocation failed\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void running_to_waiting_queue(Queue* waiting_queue, Node* running_process)
{
    // case by case : 1. when queue is empty, 2. when queue is not empty
    if (waiting_queue->tail == NULL) {  // 큐가 비어있는 경우
        waiting_queue->head = running_process;
        waiting_queue->tail = running_process;
        running_process->next = NULL;
        running_process->before = NULL;
    } else {  // 큐에 노드가 있는 경우
        waiting_queue->tail->next = running_process;
        running_process->before = waiting_queue->tail;
        waiting_queue->tail = running_process;
        running_process->next = NULL;
    }
    running_process->in_queue = 2;
    waiting_queue->size++;
}

void waiting_to_ready_queue(Queue* waiting_queue, Queue* ready_queue)
{
    Node* out = dequeue(waiting_queue);
    insert_node_queue(ready_queue, out);
}

void update_io_queue(Queue* waiting_queue, Queue* ready_queue)
{
    // if there is no process in waiting queue
    if ( waiting_queue->size == 0)
    {
        printf("No process in waiting queue to update!\n");
    }
    else    // there is process in waiting queue
    {
        Node* current_process = waiting_queue->head;
        current_process->process->IO_burst_time[current_process->process->i]--;
        if ( current_process->process->IO_burst_time[current_process->process->i] == 0 )
        {
            printf("PID %d I/O work finished. Back to ready queue\n", current_process->process->PID);
            current_process->process->i++;
            waiting_to_ready_queue(waiting_queue, ready_queue);
            if ( waiting_queue->size == 0)
                printf("Waiting queue is empty\n");
            else
                printf("New process %d begins I/O work\n", waiting_queue->head->process->PID);
        }
    }
}
void print_io_queue(Queue* q)
{
    Node* current = q->head;
    printf("=======Wait queue status=======Size: %d\n", q->size);
    if ( current == NULL)
    {
        printf("No processes in waiting queue now\n");
    }
    else
    {
        while (current != NULL)
        {
            printf("PID: %d, I/O request time: %d, I/O burst time: %d, %dth I/O execution\n",
            current->process->PID, current->process->IO_request_time[current->process->i], 
            current->process->IO_burst_time[current->process->i], current->process->i + 1);
            current = current->next;   
        }
    }
}

