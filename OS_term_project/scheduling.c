#include<stdio.h>
#include"config.h"
#include"queue.h"
#include"Gantchart.h"
#include"IO.h"
#include"evaluation.h"

#define TIME_QUANTUM 2
int remaining_quantum = TIME_QUANTUM;
Node* FCFS_sub(Queue* q, Queue* waiting_queue, int time_step, Node* running, Gantchart* cpu_chart, Gantchart* io_chart)
{
    if (running == NULL) // No cpu assign right before
    {
        if (q->size > 0) //queue is not empty
        {
            running = dequeue(q);
            running->in_queue = 0;
            printf("Process %d started at time %d\n", running->process->PID, time_step);
            add_process_to_chart(time_step, running, cpu_chart);
            return running;
        }
        else // there is no 0 arrival time process 
        {
            printf("No process is avaliable. CPU is idle.\n"); 
            add_process_to_chart(time_step, running, cpu_chart);
            return NULL;
        }
    }
    else    // Cpu was running process
    { 
        running->process->CPU_burst_time--;
        // check whether process is moving to I/O queue or not
        if ( running->process->IO_num != 0) // check only when number of I.O operation is not zero
        {
            if (running->process->IO_request_time[running->process->i] == (running->process->MAX_CPU_time - running->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running);
                running->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running, io_chart);
                printf("Process %d moved to I/o wait queue\n", running->process->PID);
                if ( q->size > 0)
                {
                    running = dequeue(q);
                    running->in_queue = 1;
                    add_process_to_chart(time_step, running, cpu_chart);
                    printf("Process %d started at time %d\n", running->process->PID, time_step);
                    return running;
                }
                else 
                {
                    printf("No process is avaliable. CPU is idle.\n");
                    add_process_to_chart(time_step, NULL, cpu_chart); 
                    return NULL;
                }
            }
        }
        if (running->process->CPU_burst_time == 0)
        {
            running->process->turnaround_time = time_step - running->process->arrival_time;
            complete_process++;
            running->in_queue = 3;
            printf("Process %d is completed at time step %d\n", running->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( q->size == 0)
            {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            running = dequeue(q);
            running->in_queue = 0;
            printf("Process %d started at time %d\n", running->process->PID, time_step);
            add_process_to_chart(time_step, running, cpu_chart);
            return running;
        }
        return running;
    }  
}

// high number -> high priority
Node* Priority_nonpreemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* current = ready_queue->head;
    int max = 0;
    if (running_process == NULL) // first time or after idle
    {
        if (ready_queue->size > 0)
        {
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->Priority;
                if (temp > max) max = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->Priority == max)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    return running_process;
                }
                else
                    current = current->next;
            }
            if (running_process == NULL)
            {
                printf("Error! No process is selected!\n");
                return NULL;
            }
        }
        else // no process in ready queue
        {
            printf("No process is avaliable. CPU is idle.\n");
            add_process_to_chart(time_step, NULL, cpu_chart); 
            return NULL;
        }
    }
    else    // already running process
    {
        running_process->process->CPU_burst_time--;
        if ( running_process->process->IO_num != 0)
        {
            if (running_process->process->IO_request_time[running_process->process->i] == (running_process->process->MAX_CPU_time - running_process->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running_process);
                running_process->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running_process, io_chart);
                printf("Process %d moved to I/o wait queue\n", running_process->process->PID);
                if ( ready_queue->size > 0)
                {
                    for ( int i = 0; i< ready_queue->size; i++)
                    {
                        int temp = current->process->Priority;
                        if (temp > max) max = temp;
                        current = current->next;
                    }
                    current = ready_queue->head;
                    for ( int i = 0; i< ready_queue->size; i++)
                    {
                        if( current->process->Priority == max)
                        {
                            running_process = remove_node_queue(ready_queue, i+1);
                            running_process->in_queue = 0;
                            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                            add_process_to_chart(time_step, running_process, cpu_chart);
                            return running_process;
                        }
                        else
                            current = current->next;
                    }
                }
                else // no process in ready queue
                {
                    printf("No process is avaliable. CPU is idle.\n");
                    add_process_to_chart(time_step, NULL, cpu_chart);
                    return NULL;
                }
            }
        }
        if (running_process->process->CPU_burst_time == 0)
        {
            running_process->process->turnaround_time = time_step - running_process->process->arrival_time;
            running_process->in_queue = 3;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running_process->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( ready_queue->size == 0) {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->Priority;
                if (temp > max) max = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->Priority == max)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
        }
        return running_process;
    }
}

// small cpu burst time -> high priority
Node* SJF_nonpreemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* current = ready_queue->head;
    int min = 11;
    if (running_process == NULL) // first time or after idle
    {
        if (ready_queue->size > 0)
        {
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->CPU_burst_time;
                if (temp < min) min = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->CPU_burst_time == min)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
            if (running_process == NULL)
            {
                printf("Error! No process is selected!\n");
                return NULL;
            }
        }
        else // no process in ready queue
        {
            printf("No process is avaliable. CPU is idle.\n");
            add_process_to_chart(time_step, NULL, cpu_chart); 
            return NULL;
        }
    }
    else    // already running process
    {
        running_process->process->CPU_burst_time--;
        // check whether process is moving to I/O queue or not
        if ( running_process->process->IO_num != 0) // check only when number of I.O operation is not zero
        {
            if (running_process->process->IO_request_time[running_process->process->i] == (running_process->process->MAX_CPU_time - running_process->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running_process);
                running_process->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running_process, io_chart);
                printf("Process %d moved to I/o wait queue\n", running_process->process->PID);
                if ( ready_queue->size > 0)
                {
                    for (int i = 0; i< ready_queue->size; i++)
                    {
                        int temp = current->process->CPU_burst_time;
                        if (temp < min) min = temp;
                        current = current->next;
                    }   
                    current = ready_queue->head;
                    for (int i = 0; i < ready_queue->size; i++)
                    {
                        if( current->process->CPU_burst_time == min)
                        {
                            running_process = remove_node_queue(ready_queue, i+1);
                            running_process->in_queue = 0;
                            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                            add_process_to_chart(time_step, running_process, cpu_chart);
                            return running_process;
                        }
                        else
                            current = current->next;
                    }
                }
                else 
                {
                    printf("No process is avaliable. CPU is idle.\n");
                    add_process_to_chart(time_step, NULL, cpu_chart); 
                    return NULL;
                }
            }
        }
        if (running_process->process->CPU_burst_time == 0)
        {
            running_process->process->turnaround_time = time_step - running_process->process->arrival_time;
            running_process->in_queue = 3;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running_process->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( ready_queue->size == 0) {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->CPU_burst_time;
                if (temp < min) min = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->CPU_burst_time == min)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
        }
        return running_process;
    }
}

Node* SJF_preemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* current = ready_queue->head;
    int min = 11;
    // No running process right now
    if ( running_process == NULL)
    {
        if (ready_queue->size > 0) // there is process in ready queue
        {
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->CPU_burst_time;
                if (temp < min) min = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->CPU_burst_time == min)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
            if (running_process == NULL)
            {
                printf("Error! No process is selected!\n");
                return NULL;
            }
        }
        else // there is no process in ready queue
        {
            printf("No process is avaliable. CPU is idle.\n");
            add_process_to_chart(time_step, NULL, cpu_chart); 
            return NULL;
        }
    }
    else // already running process
    {
        running_process->process->CPU_burst_time--;
        if ( running_process->process->IO_num != 0) // check only when number of I.O operation is not zero
        {
            if (running_process->process->IO_request_time[running_process->process->i] == (running_process->process->MAX_CPU_time - running_process->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running_process);
                running_process->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running_process, io_chart);
                printf("Process %d moved to I/o wait queue\n", running_process->process->PID);
                if ( ready_queue->size > 0)
                {
                    for (int i = 0; i< ready_queue->size; i++)
                    {
                        int temp = current->process->CPU_burst_time;
                        if (temp < min) min = temp;
                        current = current->next;
                    }   
                    current = ready_queue->head;
                    for (int i = 0; i < ready_queue->size; i++)
                    {
                        if( current->process->CPU_burst_time == min)
                        {
                            running_process = remove_node_queue(ready_queue, i+1);
                            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                            add_process_to_chart(time_step, running_process, cpu_chart);
                            return running_process;
                        }
                        else
                            current = current->next;
                    }
                }
                else 
                {
                    printf("No process is avaliable. CPU is idle.\n"); 
                    add_process_to_chart(time_step, NULL, cpu_chart);
                    return NULL;
                }
            }
        }
        if (running_process->process->CPU_burst_time == 0)      // running process is done. Getting new process from ready queue
        {
            running_process->process->turnaround_time = time_step - running_process->process->arrival_time;
            running_process->in_queue = 3;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running_process->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( ready_queue->size == 0) {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->CPU_burst_time;
                if (temp < min) min = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->CPU_burst_time == min)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
        }
        else    // running_process is not done 
        {
            // check preemption possibility
            int now = running_process->process->CPU_burst_time;
            for (int i = 0; i< ready_queue->size; i++)
                {
                    int temp = current->process->CPU_burst_time;
                    if (temp < min) min = temp;
                    current = current->next;
                }
            current = ready_queue->head;
            if ( now > min ) // there is shorter process in ready queue -> preemption
            {
                printf("Preemption occured!\n");
                insert_node_queue(ready_queue, running_process);
                running_process->in_queue = 1;
                add_endtime_to_entry(time_step, cpu_chart);
                for ( int i = 0; i < ready_queue->size; i++)
                {
                    if (current->process->CPU_burst_time == min)
                    {
                        running_process = remove_node_queue(ready_queue, i+1);
                        running_process->in_queue = 0;
                        printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                        add_process_to_chart(time_step, running_process, cpu_chart);
                        return running_process;
                    }
                    else
                        current = current->next;
                }
            }
            else             // no preemption
                return running_process;
        }
    }
}

Node* Priority_preemptive_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* current = ready_queue->head;
    int max = 0;
    if (running_process == NULL) // first time or after idle
    {
        if (ready_queue->size > 0)
        {
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->Priority;
                if (temp > max) max = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->Priority == max)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
            if (running_process == NULL)
            {
                printf("Error! No process is selected!\n");
                return NULL;
            }
        }
        else // no process in ready queue
        {
            printf("No process is avaliable. CPU is idle.\n");
            add_process_to_chart(time_step, NULL, cpu_chart);
            return NULL;
        }
    }
    else    //  already running process
    {
        running_process->process->CPU_burst_time--;
        if ( running_process->process->IO_num != 0) // check only when number of I.O operation is not zero
        {
            if (running_process->process->IO_request_time[running_process->process->i] == (running_process->process->MAX_CPU_time - running_process->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running_process);
                running_process->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running_process, io_chart);
                printf("Process %d moved to I/o wait queue\n", running_process->process->PID);
                if ( ready_queue->size > 0)
                {
                    for (int i = 0; i< ready_queue->size; i++)
                    {
                        int temp = current->process->CPU_burst_time;
                        if (temp > max) max = temp;
                        current = current->next;
                    }   
                    current = ready_queue->head;
                    for (int i = 0; i < ready_queue->size; i++)
                    {
                        if( current->process->CPU_burst_time == max)
                        {
                            running_process = remove_node_queue(ready_queue, i+1);
                            running_process->in_queue = 0;
                            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                            add_process_to_chart(time_step, running_process, cpu_chart);
                            return running_process;
                        }
                        else
                            current = current->next;
                    }
                }
                else 
                {
                    printf("No process is avaliable. CPU is idle.\n");
                    add_process_to_chart(time_step, NULL, cpu_chart); 
                    return NULL;
                }
            }
        }
        if (running_process->process->CPU_burst_time == 0)
        {
            running_process->process->turnaround_time = time_step - running_process->process->arrival_time;
            running_process->in_queue = 3;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running_process->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( ready_queue->size == 0) {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            for (int i = 0; i< ready_queue->size; i++)
            {
                int temp = current->process->Priority;
                if (temp > max) max = temp;
                current = current->next;
            }
            current = ready_queue->head;
            for (int i = 0; i < ready_queue->size; i++)
            {
                if( current->process->Priority == max)
                {
                    running_process = remove_node_queue(ready_queue, i+1);
                    running_process->in_queue = 0;
                    printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                    add_process_to_chart(time_step, running_process, cpu_chart);
                    return running_process;
                }
                else
                    current = current->next;
            }
        }
        else    // running process is not done -> compare with ready queue
        {
            int now = running_process->process->Priority;
            for (int i = 0; i< ready_queue->size; i++)
                {
                    int temp = current->process->Priority;
                    if (temp > max) max = temp;
                    current = current->next;
                }
            current = ready_queue->head;
            if ( now < max ) // there is shorter process in ready queue -> preemption
            {
                printf("Preemption occured!\n");
                insert_node_queue(ready_queue, running_process);
                running_process->in_queue = 1;
                add_endtime_to_entry(time_step, cpu_chart);
                for ( int i = 0; i < ready_queue->size; i++)
                {
                    if (current->process->Priority == max)
                    {
                        running_process = remove_node_queue(ready_queue, i+1);
                        running_process->in_queue = 0;
                        printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                        add_process_to_chart(time_step, running_process, cpu_chart);
                        return running_process;
                    }
                    else
                        current = current->next;
                }
            }
            else             // no preemption
                return running_process; 
        }
    }
}

Node* Round_Robin_sub(Queue* ready_queue, Queue* waiting_queue, int time_step, Node* running_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    if (running_process == NULL)
    {
        if (ready_queue->size > 0 )
        {
            running_process = dequeue(ready_queue);
            running_process->in_queue = 0;
            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
            add_process_to_chart(time_step, running_process, cpu_chart);

            remaining_quantum = 2;
            return running_process;
        }
        else // there is no process in ready queue
        {
            printf("No process is avaliable. CPU is idle.\n");
            add_process_to_chart(time_step, NULL, cpu_chart); 
            return NULL;
        }
    }
    else    // running_process exists
    {
        running_process->process->CPU_burst_time--;
        remaining_quantum--;
        if ( running_process->process->IO_num != 0)
        {
            if ( running_process->process->IO_request_time[running_process->process->i] == (running_process->process->MAX_CPU_time - running_process->process->CPU_burst_time))
            {
                running_to_waiting_queue(waiting_queue, running_process);
                running_process->in_queue = 2;
                add_endtime_to_entry(time_step, cpu_chart);
                add_process_to_io_chart(time_step, waiting_queue, running_process, io_chart);
                printf("Process %d moved to I/o wait queue\n", running_process->process->PID);
                if ( ready_queue->size == 0)
                {
                    add_process_to_chart(time_step, NULL, cpu_chart);
                    return NULL;
                }
                running_process = dequeue(ready_queue);
                running_process->in_queue = 0;
                remaining_quantum = 2;
                printf("Process %d started at time %d\n", running_process->process->PID, time_step);
                add_process_to_chart(time_step, running_process, cpu_chart);
                return running_process;
            }
        }
        if (running_process->process->CPU_burst_time == 0) // process ended
        {
            running_process->process->turnaround_time = time_step - running_process->process->arrival_time;
            running_process->in_queue = 3;
            complete_process++;
            printf("Process %d is completed at time step %d\n", running_process->process->PID, time_step);
            add_endtime_to_entry(time_step, cpu_chart);
            if ( ready_queue->size == 0)
            {
                add_process_to_chart(time_step, NULL, cpu_chart);
                return NULL;
            }
            running_process = dequeue(ready_queue);
            running_process->in_queue = 0;
            remaining_quantum = 2;
            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
            add_process_to_chart(time_step, running_process, cpu_chart);
            return running_process;
        }
        else if (( remaining_quantum == 0) && ( ready_queue->size != 0)) // time quantum is over -> go back to ready queue
        {
            printf("Process %d out of time quantum\n", running_process->process->PID);
            add_endtime_to_entry(time_step, cpu_chart);
            running_process->in_queue = 1;
            insert_node_queue(ready_queue, running_process);
            running_process = dequeue(ready_queue);
            remaining_quantum = 2;
            running_process->in_queue = 0;
            printf("Process %d started at time %d\n", running_process->process->PID, time_step);
            add_process_to_chart(time_step, running_process, cpu_chart);
            return running_process;
        }
        else    // stil remaining time quantum & cpu burst time
            return running_process;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FCFS(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        if ( temp != waiting_queue->size)       // waiting queue size different ->To check i/o work is done
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        running_process = FCFS_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);  // cpu_status의 주소 전달
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}

void Priority_nonpreemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        if ( temp != waiting_queue->size)
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        running_process = Priority_nonpreemptive_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}

void SJF_nonpreemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        if ( temp != waiting_queue->size)
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        running_process = SJF_nonpreemptive_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}

void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        if ( temp != waiting_queue->size)
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        running_process = SJF_preemptive_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}

void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        if ( temp != waiting_queue->size)
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        running_process = Priority_preemptive_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}
// Round robin & FCFS
void Round_Robin(Queue* ready_queue, Queue* waiting_queue, Node** pcb_array, int num_process, Gantchart* cpu_chart, Gantchart* io_chart)
{
    Node* running_process = NULL;
    while( complete_process != num_process)
    {
        int temp = waiting_queue->size;
        printf("====================Time step: %d====================\n", time_step);
        if (time_step != 0) {
            update_queue(pcb_array, ready_queue, time_step, num_process);
            update_io_queue(waiting_queue, ready_queue);
        }
        if ( temp != waiting_queue->size)
        {
            add_endtime_to_entry(time_step, io_chart);
            if (waiting_queue->size != 0)
                add_process_to_io_chart(time_step, waiting_queue, waiting_queue->head, io_chart);
        }
        print_queue(ready_queue);
        print_io_queue(waiting_queue);
        running_process = Round_Robin_sub(ready_queue, waiting_queue, time_step, running_process, cpu_chart, io_chart);
        update_waiting_time(num_process, pcb_array);
        printf("complete process: %d\n", complete_process);
        time_step++;
    }
}