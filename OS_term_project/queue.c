#include<stdlib.h>
#include"config.h"

Queue* create_queue(int number_of_process, Node** node_array)
{
    if (number_of_process < 1)
    {
        printf("Function create_queue: number of process is less than 1\n");
        return NULL;
    }
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if ( !q ) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    // put nodes into the queue only when arrival time is 0
    for (int i = 0; i< number_of_process; i++)
    {
        if (node_array[i]->process->arrival_time == 0)
        {
            if (q->head == NULL) // IF inserting node first time
            {
                q->head = node_array[i];
                q->tail = node_array[i];
            }
            else 
            {
                q->tail->next = node_array[i];
                node_array[i]->before = q->tail;
                q->tail = node_array[i];
            }
        }
        q->size = q->size + 1;
    }
    return q;
}
Node* create_node(Process* process)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node){
        printf("Memory allocation failed\n");
        return NULL;
    }
    node->process = process;
    node->before = NULL;
    node->next = NULL;
    return node;
}