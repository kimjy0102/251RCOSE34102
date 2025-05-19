#include <stdio.h>
#include "config.h"

// in_queue = 0 : running
// in_queue = 1 : in ready_queue
// in_queue = 2 : in waiting_queue
// in_queue = 3 : terminated
void print_turnaround_time(int num_process, Node** node_array)
{
    printf("=====Turnaround Time=====\n");
    for (int i = 0; i < num_process ; i++ )
    {
        printf("PID: %d Time: %d\n", node_array[i]->process->PID, node_array[i]->process->turnaround_time);
    }
}

void print_waiting_time(int num_process, Node** node_array)
{
    printf("=====Waiting Time=====\n");
    for (int i = 0; i < num_process ; i++ )
    {
        printf("PID: %d Time: %d\n", node_array[i]->process->PID, node_array[i]->process->waiting_time);
    }
}

void update_waiting_time(int num_process, Node** node_array)
{
    for ( int i = 0; i < num_process; i++)
    {
        if (node_array[i]->in_queue == 1)
            node_array[i]->process->waiting_time++;
    }
}