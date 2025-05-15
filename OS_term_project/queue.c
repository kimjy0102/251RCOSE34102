#include<stdio.h>
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
    if (!q) 
    {
        printf("Queue Memory allocation failed\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    
    // put nodes into the queue only when arrival time is 0
    for (int i = 0; i < number_of_process; i++)
    {
        if (node_array[i]->process->arrival_time == 0)
        {
            if (q->head == NULL) // IF inserting node first time
            {
                q->head = node_array[i];
                q->tail = node_array[i];
                node_array[i]->in_queue = 1;
                q->size++;  // size 증가는 실제로 노드가 추가될 때만
            }
            else 
            {
                q->tail->next = node_array[i];
                node_array[i]->before = q->tail;
                q->tail = node_array[i];
                node_array[i]->in_queue = 1;
                q->size++;  // size 증가는 실제로 노드가 추가될 때만
            }
        }
    }
    return q;
}

Node* create_node(Process* process)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node){
        printf("Node Memory allocation failed\n");
        free(node->process);
        return NULL;
    }
    node->process = process;
    node->before = NULL;
    node->next = NULL;
    node->in_queue = 0;
    return node;
}
// location 필요 여부.. queue자체를 ready queue로 만들면 location 필요 없나? 
// 아니면 arrival time에 따라 넣어줘야 하는가? -> arrival time에 따라 들어가니까 location 필요 없을 듯
void insert_node_queue(Queue* q, Node* new_node) 
{
    if (q->tail == NULL) {  // 큐가 비어있는 경우
        q->head = new_node;
        q->tail = new_node;
        new_node->next = NULL;
        new_node->before = NULL;
    } else {  // 큐에 노드가 있는 경우
        q->tail->next = new_node;
        new_node->before = q->tail;
        q->tail = new_node;
        new_node->next = NULL;
    }
    new_node->in_queue = 1;
    q->size++;
    printf("Process %d is inserted.\n", new_node->process->PID);
}
Node* dequeue(Queue* q) // Remove only the first node in queue
{   
    if (q->head == NULL)
    {
        printf("Ready Queue is empty\n");
        return NULL;
    }
    else if (q->head->next == NULL) // when only one node is in queue
    {
        Node*temp = q->head;
        q->head = q->head->next;
        q->tail = NULL;
        q->size--;
        return temp;
    }
    else 
    {
        Node* temp = q->head;
        q->head = q->head->next;
        q->head->before = NULL;
        q->size--;
        temp->in_queue = 0;
        printf("Process %d is removed from ready queue.\n", temp->process->PID);
        return temp;
    }
}
Node* remove_node_queue(Queue* q, int location)
{
    if ( location > q->size  || location < 1)
    {
        printf("Location is out of boundary!\n");
        return NULL;
    }
    
    Node* current = q->head;
    Node* prev = NULL;

    for(int i = 1; i<location; i++)
    {
        prev = current;
        current = current->next;
    }
    if (q->size == 1)
    {
        q->head = NULL;
        q->tail = NULL;
    }
    else
    {
        if ( current == q->head) // if first node removal
        {
            q->head = q->head->next;
            q->head->before = NULL;
        }
        else if ( current == q->tail )  // last node removal
        {
            q->tail = prev;
            q->tail->next = NULL;
        }
        else                            // normal removal
        {
            prev->next = current->next; 
            current->next->before = prev;
        }
    }
    current->next = NULL;
    current->before = NULL;
    current->in_queue = 0;
    q->size--;
    return current;
}

void print_queue(Queue* q)
{
    Node* current = q->head;
    printf("========Queue status======== Size: %d\n", q->size);
    if ( current == NULL)
    {
        printf("No processes in ready queue now\n");
    }
    else
    {
        while (current != NULL)
        {
            printf("PID: %d, Arrival: %d, CPU time: %d, Priority: %d\n",
                current->process->PID, current->process->arrival_time, current->process->CPU_burst_time, current->process->Priority);
            current = current->next;   
        }
    }
}
void update_queue(Node** node_array, Queue* q, int time_step, int num_process)
{
    for (int i = 0; i < num_process; i++ )
    {
        if ( time_step == node_array[i]->process->arrival_time)
        {
            insert_node_queue(q, node_array[i]);
            printf("Process %d is inserted into ready queue\n", node_array[i]->process->PID);
        }
    }
}
void print_node_array(Node** node_array, int num_process)
{
    printf("===========Node array==========\n");
    for (int i = 0; i< num_process; i++)
    {
        printf("Process: %d\n", i+1);
        printf("PID: %d, Arrival time : %d, CPU burst time: %d, Priority: %d\n", node_array[i]->process->PID, node_array[i]->process->arrival_time, node_array[i]->process->CPU_burst_time, node_array[i]->process->Priority);
    }

}
void clean_queue(Queue* q)
{
    free(q);
}