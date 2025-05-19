#ifndef GANTCHART_H

#define GANTCHART_H
#include"config.h"
#define MAX_LENGTH 100

typedef struct Gantentry {
    int PID;
    int start_time;
    int end_time;
    struct Gantentry* next;
} Gantentry;

typedef struct Gantchart {
    int size;
    struct Gantentry* head;
    struct Gantentry* tail;
} Gantchart;
void add_process_to_chart(int time_step, Node* running, Gantchart* chart);
void add_process_to_io_chart(int time_step, Queue* wait_queue, Node* running, Gantchart* chart);
Gantchart* init_chart(void);
void add_endtime_to_entry(int time_step, Gantchart* chart);
void print_gantchart(Gantchart* chart, Gantchart* io_chart);
void free_gantchart(Gantchart* chart);
#endif