#include "config.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "IO.h"
// just one process creation
Process* create_one_process(int pid)
{
    Process* new_process = (Process*)malloc(sizeof(Process));
    if (new_process == NULL)
    {
        printf("Process Memory allocation failed.\n");
        return NULL;
    }
    int priority = rand() % 10 + 1; // Priority : 1 ~ 10
    int arrival_time = rand() % 10; // Arrival time : 0 ~ 9
    int CPU_burst_time = rand() % 10 + 1; // CPU : 1 ~ 10
    int num_IO = create_num_IO(CPU_burst_time);
    int* IO_burst_time = create_burst_time(num_IO);
    int* IO_request_time = create_request_time(num_IO, CPU_burst_time);
    new_process->arrival_time = arrival_time;
    new_process->Priority = priority;
    new_process->CPU_burst_time = CPU_burst_time;
    new_process->PID = pid;
    new_process->turnaround_time = 0;
    new_process->waiting_time = 0;
    // IO
    new_process->MAX_CPU_time = CPU_burst_time;
    new_process->IO_num = num_IO;
    new_process->IO_burst_time = IO_burst_time;
    new_process->IO_request_time = IO_request_time;
    new_process->i = 0;
    return new_process;
}

// Main process creation function. Create processes and all of them are stored as node pointers' array
Node** create_processes(int num_process)
{
    Node** node_array = (Node**)malloc(sizeof(Node*) * (num_process));
    if (node_array == NULL)
    {
        printf("Node array Memory allocation failed.\n");
        return NULL;
    }
    for (int i = 0; i< num_process; i++)
    {
        Process* process = create_one_process(i + 1);
        if (process == NULL)
        {
            for (int j = 0; j<i ; j++)
            {
                free(node_array[j]->process);
                free(node_array[j]);
            }
            free(node_array);
            return NULL;
        }
        Node* node = create_node(process);
        node_array[i] = node;
    }
    return node_array;
}

void clean_all(Node** node_array, int num_process)
{
    if (node_array == NULL) return;
    for (int i = 0; i<num_process; i++)
    {
        if(node_array[i] != NULL)
        {
            if (node_array[i]->process != NULL)
            {
                if (node_array[i]->process->IO_burst_time != NULL)
                {
                    free(node_array[i]->process->IO_burst_time);
                    node_array[i]->process->IO_burst_time = NULL;
                }
                if (node_array[i]->process->IO_request_time != NULL)
                {
                    free(node_array[i]->process->IO_request_time);
                    node_array[i]->process->IO_request_time = NULL;
                }
                free(node_array[i]->process);
                node_array[i]->process = NULL;
            }
            free(node_array[i]);
            node_array[i] = NULL;
        }
    }
    free(node_array);
    node_array = NULL;
}