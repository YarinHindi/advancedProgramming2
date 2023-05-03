#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priorityQ.h"



int compare(QueueElement *a, QueueElement *b) {
    return a->priority - b->priority;
}

void swap(QueueElement **a, QueueElement **b) {
    QueueElement *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue *queue, int index) {
    int parent = (index - 1) / 2;
    if (index > 0 && compare(queue->data[index], queue->data[parent]) < 0) {
        swap(&queue->data[index], &queue->data[parent]);
        heapify_up(queue, parent);
    }
}

void heapify_down(PriorityQueue *queue, int index) {
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    int min_index = index;
    if (left_child < queue->size && compare(queue->data[left_child], queue->data[min_index]) < 0) {
        min_index = left_child;
    }
    if (right_child < queue->size && compare(queue->data[right_child], queue->data[min_index]) < 0) {
        min_index = right_child;
    }
    if (min_index != index) {
        swap(&queue->data[index], &queue->data[min_index]);
        heapify_down(queue, min_index);
    }
}

void enqueue(PriorityQueue *queue, char *str, int priority) {
    if (queue->size == MAX_QUEUE_SIZE) {
        printf("Queue is full, cannot enqueue.\n");
        return;
    }
    QueueElement *element = (QueueElement *)malloc(sizeof(QueueElement));
    strcpy(element->data, str);
    element->priority = priority;
    queue->data[queue->size] = element;
    queue->size++;
    heapify_up(queue, queue->size - 1);
}

QueueElement *dequeue(PriorityQueue *queue) {
    if (queue->size == 0) {
        printf("Queue is empty, cannot dequeue.\n");
        return NULL;
    }
    QueueElement *element = queue->data[0];
    
    queue->data[0] = queue->data[queue->size - 1];
    queue->size--;
    heapify_down(queue, 0);
    return element;
}
void freeQ(PriorityQueue *queue){
    while (queue->size >0)
    {
        QueueElement *element = dequeue(&queue);
        free(element);
    }
    
}


