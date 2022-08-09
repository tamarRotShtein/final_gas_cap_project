#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


//implement queue synchronic functions

node * createNode(void * data, int size){
    node* my_node=(node*)malloc(sizeof(node));
    my_node->data=data;
    my_node->next=NULL;
    return my_node;
}

queue* createQueue(int capacity){
    queue *q=(queue*)malloc(sizeof(queue));
    initQueue(q,capacity);
    return q;
}
void initQueue(queue* q,int capacity){
    q->capacity=capacity;
    q->size=0;
    q->head=NULL;
    q->tail=NULL;
    sem_init(&(q->mutex),0,1);//to do sync
    sem_init(&(q->is_full_mutex),0,capacity);//to check if there is place
    while(capacity)
    {
        sem_wait(&(q->is_full_mutex));
        capacity--;
    }
    return;
}
void freeNode(node* node){
    if(node==NULL)
        return;
//    if(node->data!=N)
//    {free(node->data);
//    node->data=0;
//    }
    free(node);
    node=0;
}
void freeQueue(queue* q){
    if(q==NULL)
        return;
    sem_destroy(&q->mutex);
    sem_destroy(&q->is_full_mutex);
    while(!isEmpty(q)){
        freeNode(dequeue(q));
    }
    free(q);
}
int getCapacity(queue* q){
    return q->capacity;
}
int getSize(queue* q){
    return q->size;
}
void enqueu(queue* q,node* data){

    sem_wait(&q->mutex);//lock sem

    if(q->size==q->capacity){
        return;
    }
    if(q->head==NULL){
        q->head=q->tail=data;
    }
    else{
        q->tail->next=data;
        q->tail=data;
    }
    q->size++;
    sem_post(&q->mutex);//free sem
    sem_post(&q->is_full_mutex);

}
node* dequeue(queue* q){
    node* temp;
    sem_wait(&q->is_full_mutex);
    sem_wait(&q->mutex);//lock sem
    temp=q->head;
    q->head=q->head->next;
    q->size--;
    if(isEmpty(q))
        q->head=q->tail=NULL;
    sem_post(&q->mutex);//free sem
    return temp;
}
int isEmpty(queue* q){
    return q->size==0;
}
int isFull(queue *q){
    sem_wait(&q->mutex);//lock sem
    int isFull=q->size==q->capacity;
    sem_post(&q->mutex);//free sem
    return isFull;
}

