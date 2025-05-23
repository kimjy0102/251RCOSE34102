#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "queue.h"
#include "process.h"
#include "scheduling.h"
#include "Gantchart.h"
#include "IO.h"
#include "evaluation.h"
int time_step = 0;
int complete_process = 0;
int main(void)
{
    // initial process assignment
    srand(time(NULL)); 
    int num_process = (rand() % MAX_PROCESS) + 1;    
    printf("Number of process : %d created\n", num_process);
    Node** PCB_array = create_processes(num_process);
    if ( PCB_array == NULL)
    {
        printf("Failed to create processes\n");
        return 1;
    }
    printf("PCB_array created\n");
    print_node_array(PCB_array, num_process);
    // ready queue
    Queue* ready_queue = create_queue(num_process, PCB_array);
    if ( ready_queue == NULL)
    {
        printf("Failed to create ready queue.\n");
        clean_all(PCB_array, num_process);
        return 1;
    }
    printf("Ready queue created\n");
    // I/O waiting queue
    Queue* waiting_queue = create_waiting_queue();
    if ( waiting_queue == NULL)
    {
        printf("Failed to create waiting queue.\n");
        clean_all(PCB_array, num_process);
        return 1;
    }
    printf("Waiting queue created\n");
    // scheduling begins
    int mode = 0;
    Gantchart* cpu_chart = init_chart();
    Gantchart* io_chart = init_chart();
    printf("Select CPU Scheduling Algorithm\n");
    printf("1: FCFS 2: Non-preemptive SJF 3: Preemptie SJF 4: Non-preemptive priority 5: Preemptive priority 6: Round-Robin\n");
    printf("Scheduling mode: ");
    scanf(" %d", &mode);
    switch (mode)
    {
    case  1:
        FCFS(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    case 2:
        SJF_nonpreemptive(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    case 3:
        SJF_preemptive(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    case 4:
        Priority_nonpreemptive(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    case 5:
        Priority_preemptive(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    case 6:
        Round_Robin(ready_queue, waiting_queue, PCB_array, num_process, cpu_chart, io_chart);
        break;
    default:
        printf("Wrong Number inserted!\n");
        return 0;
    }
    printf("All process terminated at time_step %d\n", time_step - 1);
    print_gantchart(cpu_chart, io_chart);
    print_turnaround_time(num_process, PCB_array);
    print_waiting_time(num_process, PCB_array);
    clean_all(PCB_array, num_process);
    clean_queue(ready_queue);
    clean_queue(waiting_queue);
    return 0;
}
