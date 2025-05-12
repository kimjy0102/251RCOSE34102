#include<stdio.h>
#include"config.h"
#include"queue.h"
#include"Gantchart.h"
Node* FCFS_sub(Queue* q, int time_step, Node* running, STATUS* cpu_status, Gantchart* chart)
{
    if (running == NULL) // No cpu assign right before
    {
        if (q->size > 0) //queue is not empty
        {
            running = dequeue(q);
            printf("Process %d started at time %d\n", running->process->PID, time_step);
            add_process_to_chart(time_step, running, chart);
            running->process->waiting_time = time_step - running->process->arrival_time;
            *cpu_status = CPU_BUSY;
            return running;
        }
        else // there is no 0 arrival time process 
        {
            printf("No process is avaliable. CPU is idle.\n");
            *cpu_status = CPU_IDLE; 
            return NULL;
        }
    }
    else    // Cpu was running process
    {
        running->process->CPU_burst_time--;
        if (running->process->CPU_burst_time == 0)
        {
            running->process->turnaround_time = time_step + 1 - running->process->arrival_time;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running->process->PID, time_step);
            add_endtime_to_entry(time_step, chart);
            if ( q->size == 0)
            {
                return NULL;
            }
            running = dequeue(q);
            printf("Process %d started at time %d\n", running->process->PID, time_step);
            *cpu_status = CPU_BUSY;
            return running;
        }
        return running;
    }  
}

void FCFS(Queue* ready_queue, Node** pcb_array, int num_process, Gantchart* chart)
{
    STATUS cpu_status = CPU_IDLE;
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        printf("=====Time step: %d=====\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
        }
        print_queue(ready_queue);
        running_process = FCFS_sub(ready_queue, time_step, running_process, &cpu_status, chart);  // cpu_status의 주소 전달
        time_step++;
    }
}