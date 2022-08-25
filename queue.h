#ifndef QUEUE_H
#define QUEUE_H

#include  <semaphore.h>
typedef struct node{
    void* data;
    struct node* next;
}node;

typedef struct queue{
    sem_t mutex;
    sem_t is_full_mutex;
    unsigned int size;
    unsigned int capacity;
    struct node* head;
    struct node* tail;
}queue;

node * createNode(void * data);
queue* createQueue(int capacity);
void initQueue(queue*,int capacity);
void freeNode(node* node);
void freeQueue(queue* q);
int getCapacity(queue* q);
int getSize(queue* q);
void enqueu(queue* q,node* data);
node* dequeue(queue* q);
int isEmpty(queue* q);
int isFull(queue *q);
#endif // QUEUE_H
