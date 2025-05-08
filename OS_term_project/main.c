#include <stdio.h>
#include "config.h"
#include "queue.h"
#include "process.h"

int time_step = 0;
int complete_process = 0;

int main(void)
{
    int num_process = (rand() % MAX_PROCESS) + 1;    
    printf("Number of process : %d created\n");
    Node** PCB_array = create_processes(num_process);
    if ( PCB_array == NULL)
    {
        printf("Failed to create processes\n");
    }
    // ready queue
    Queue* ready_queue = create_queue(num_process, PCB_array);
    if ( ready_queue == NULL)
    {
        printf("Failed to create ready queue.\n");
    }
    print_queue;
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        printf("=====Time step: %d=====\n", time_step);
        update_queue(PCB_array, ready_queue, time_step);
        
    }
}
