#include<stdio.h>
#include<stdlib.h>
#include"queue.c"
#include"config.h"
#include"Gantchart.h"

Gantchart* init_chart(int num_process)
{
    Gantchart* chart = (Gantchart*)malloc(sizeof(Gantchart));
    if (!chart)
    {
        printf("Gantchart Memory allocation failed\n");
        return NULL;
    }
    chart->size = 0;
    chart->head = NULL;
    chart->tail = NULL;
    return chart;
}
// add start time and process 
void add_process_to_chart(int time_step, Node* running, Gantchart* chart)
{
    // create Gant entry
    Gantentry* entry = (Gantentry*)malloc(sizeof(Gantentry));
    entry->end_time = 0;
    entry->start_time = time_step;
    entry->PID = running->process->PID;
    entry->next = NULL;
    // if no entries in chart
    if (chart->size == 0)
    {
        chart->head = entry;
        chart->tail = entry;
        chart->size++;
    }
    else // already has entries
    {
        chart->tail->next = entry;
        chart->tail = chart->tail->next;
        chart->size++;
    }
}
// add end time to current process
void add_endtime_to_entry(int time_step, Gantchart* chart)
{
    chart->tail->end_time = time_step;
}


// add cpu idle moment
// print gantchart
void print_gantchart(Gantchart* chart)
{
    if (!chart || chart->size == 0)
    {
        printf("Gant chart is empty!\n");
    }
    Gantentry* current = chart->head;
    printf("==========Gant Chart==========\n");
    int width = 0;
    
}
// free gantchart