#include<stdio.h>
#include"config.h"

Node* FCFS(Queue* q, int time_step, Node* running)
{
    if (running == NULL) // No cpu assign right before
    {
        if (q->size > 0) //queue is not empty
        {
            Node* running = dequeue(q);
            printf("Process %d started at time %d\n", running->process->PID, time_step);
            return running;
        }
        else // there is no 0 arrival time process 
        {
            printf("No process is avaliable. CPU is idle.\n");
            return NULL;
        }
    }
    else    // Cpu was running process
    {
        running->process->CPU_burst_time--;
        if (running->process->CPU_burst_time == 0)
        {
            
        }
    }  
}
