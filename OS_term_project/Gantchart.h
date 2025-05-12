#ifndef GANTCHART_H

#define GANTCHART_H
#include"config.h"
#define MAX_LENGTH 100

typedef struct Gantentry {
    int PID;
    int start_time;
    int end_time;
    Gantentry* next;
} Gantentry;

typedef struct Gantchart {
    int size;
    Gantentry* head;
    Gantentry* tail;
} Gantchart;
void add_process_to_chart(int time_step, Node* running, Gantchart* chart);
Gantchart* init_chart(int num_process);
void add_endtime_to_entry(int time_step, Gantchart* chart);
#endif