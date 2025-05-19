#include<stdio.h>
#include<stdlib.h>
#include"config.h"
#include"Gantchart.h"

Gantchart* init_chart(void)
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
    if ( running == NULL)
        entry->PID = -1;
    else
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
// IO is working different, so make other function for I/O Gant chart
void add_process_to_io_chart(int time_step, Queue* wait_queue, Node* running, Gantchart* chart)
{
    if ( wait_queue->head != running) // if there is already running process in i/o
    {
        return;
    }
    else
    {
        Gantentry* entry = (Gantentry*)malloc(sizeof(Gantentry));
        entry->end_time = 0;
        entry->start_time = time_step;
        if ( running == NULL)
            entry->PID = -1;
        else
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
}

// add end time to current process
void add_endtime_to_entry(int time_step, Gantchart* chart)
{
    chart->tail->end_time = time_step;
}

// print gantchart
void print_gantchart(Gantchart* chart, Gantchart* io_chart)
{
    if (!chart || chart->size == 0)
    {
        printf("Gant chart is empty!\n");
        return;
    }
    Gantentry* current = chart->head;
    printf("\n============Gant Chart============\n");
    
    // Print time line
    printf("Time: ");
    while (current != chart->tail)
    {
        printf("%-5d", current->start_time);
        if (current->next == chart->tail)
            printf("%-5d", current->end_time);
        current = current->next;
    }
    printf("\n");
    
    // Print process line
    current = chart->head;
    printf("Proc: ");
    while (current != chart->tail)
    {
        if (current->PID == -1)
            printf("IDLE ");
        else
            printf("P%-4d", current->PID);
        if (current->next == chart->tail)
            printf("    ");
        current = current->next;
    }
    printf("\n");
    
    
    // Print I/O chart if exists
    if (io_chart && io_chart->size > 0)
    {
        printf("\n==========I/O Gant Chart==========\n");
        current = io_chart->head;
        
        // Print I/O time line
        printf("Time: ");
        while (current != NULL)
        {
            printf("%-5d", current->start_time);
            printf("%-5d", current->end_time);
            current = current->next;
        }
        printf("\n");
        
        // Print I/O process line
        current = io_chart->head;
        printf("I/O:  ");
        while (current != NULL)
        {
            if (current->PID == -1)
                printf("IDLE ");
            else
                printf("P%-4d", current->PID);
            printf("     ");  // Add space for end time
            current = current->next;
        }
        printf("\n");
    }
    printf("==================================\n\n");
}


// free gantchart

void free_gantchart(Gantchart* chart)
{
    if (!chart) return;  

    Gantentry* current = chart->head;
    while (current != NULL)
    {
        Gantentry* temp = current;
        current = current->next;
        free(temp);
    }   
    free(chart);
}