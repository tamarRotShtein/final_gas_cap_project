#ifndef STAGE_H
#define STAGE_H
#include "queue.h"
#include <pthread.h>

//struct to stage in pipeline
typedef struct stage{

    void* (*my_task)(void*);
    pthread_t thread;
    int isActive;
    queue* sourseQu;
    queue* destQu;
    struct stage * next_stage;
    int is_enqueue;
    int is_dequeue;
}stage;


#endif // STAGE_H
