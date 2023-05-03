#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "priorityQ.h"

#define DATA_BUFFER_SIZE 1024
     PriorityQueue queue = {0};
     char *mode;
typedef struct {
    char buffer[DATA_BUFFER_SIZE];
    int size;
    int key;
	int num;
} Data;



typedef struct {
    Data *buffer;
    int capacity;
    int size;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} ThreadPool;



void thread_pool_init(ThreadPool *pool, int capacity) {
    pool->buffer = malloc(capacity * sizeof(Data));
    pool->capacity = capacity;
    pool->size = 0;
    pool->front = 0;
    pool->rear = 0;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);
}

void thread_pool_enqueue(ThreadPool *pool, const Data *data) {
    pthread_mutex_lock(&pool->mutex);
    while (pool->size >= pool->capacity) {
        pthread_cond_wait(&pool->not_full, &pool->mutex);
    }
    memcpy(&pool->buffer[pool->rear], data, sizeof(Data));
    pool->rear = (pool->rear + 1) % pool->capacity;
    pool->size++;
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->mutex);
}

void thread_pool_dequeue(ThreadPool *pool, Data *data) {
    pthread_mutex_lock(&pool->mutex);
    while (pool->size <= 0) {
        pthread_cond_wait(&pool->not_empty, &pool->mutex);
    }
    memcpy(data, &pool->buffer[pool->front], sizeof(Data));
    pool->front = (pool->front + 1) % pool->capacity;
    pool->size--;
    pthread_cond_signal(&pool->not_full);
    pthread_mutex_unlock(&pool->mutex);
}

void thread_pool_destroy(ThreadPool *pool) {
    free(pool->buffer);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_full);
}

void *worker_thread(void *arg) {
	
	ThreadPool *pool = (ThreadPool *) arg;
	Data data;
    int is_empty = 0;
    while (!is_empty) {
		
        thread_pool_dequeue(pool, &data);
         if (strcmp(mode, "-e") == 0){
                encrypt(data.buffer, data.key);
            }else{
                decrypt(data.buffer, data.key);
            }
		pthread_mutex_lock(&pool->mutex);
		enqueue(&queue,data.buffer,data.num);
		pthread_mutex_unlock(&pool->mutex);
	
     
        if (pool->size == 0) {
            is_empty = 1;
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{

    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	int num_task  = 0;  
    if (argc != 3 || (strcmp(argv[2], "-e") != 0 && strcmp(argv[2], "-d") != 0))
    {
        printf("usage: key < file \n");
        printf("!! data more than 1024 char will be ignored !!\n");
        return 0;
    }

    int key = atoi(argv[1]);
    mode  = argv[2];


    ThreadPool pool;
    thread_pool_init(&pool, num_cores);

    pthread_t threads[num_cores];


    char c;
    int counter = 0;
    char data[DATA_BUFFER_SIZE];
	int times_inside = 0;

    while ((c = getchar()) != EOF)
    {
        data[counter] = c;
        counter++;

        if (counter == DATA_BUFFER_SIZE){
            Data d = {0};
			
			if (times_inside < num_cores){
				pthread_create(&threads[times_inside], NULL, worker_thread, &pool);
				times_inside++;
			}
            memcpy(d.buffer, data, DATA_BUFFER_SIZE);
            d.size = DATA_BUFFER_SIZE;
            d.key = key;
			d.num = num_task;
			num_task++;
            thread_pool_enqueue(&pool, &d);
            counter = 0;
        }
    }
    
    if (counter > 0)
    {
		if (times_inside < num_cores){
			
				pthread_create(&threads[times_inside], NULL, worker_thread, &pool);
				times_inside++;
			}
        Data d = {0};
        memcpy(d.buffer, data, counter);
        d.size = counter;
        d.key = key;
		d.num = num_task+1;
        thread_pool_enqueue(&pool, &d);
    }

	int should_wait = 0;
	if (times_inside < num_cores){
		should_wait = times_inside;
	}else{
		should_wait = num_cores;
	}
    for (int i = 0; i < should_wait; i++) {
        pthread_join(threads[i], NULL);
    }
    thread_pool_destroy(&pool);

	    while (queue.size > 0) {
        QueueElement *element = dequeue(&queue);
        printf("%s", element->data);
    }
	freeQ(&queue);
	
    return 0;
}
