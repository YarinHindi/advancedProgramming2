#ifndef PRIORITYQ_H
#define PRIORITYQ_H

#define MAX_QUEUE_SIZE 1000

typedef struct {
    char data[1024];
    int priority;
} QueueElement;

typedef struct {
    QueueElement *data[MAX_QUEUE_SIZE];
    int size;
} PriorityQueue;

int compare(QueueElement *a, QueueElement *b);
void swap(QueueElement **a, QueueElement **b);
void heapify_up(PriorityQueue *queue, int index);
void heapify_down(PriorityQueue *queue, int index);
void enqueue(PriorityQueue *queue, char *str, int priority);
QueueElement *dequeue(PriorityQueue *queue);
void freeQ(PriorityQueue *queue);

#endif