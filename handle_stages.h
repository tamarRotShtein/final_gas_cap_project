#ifndef HANDLE_STAGES_H
#define HANDLE_STAGES_H
#include <stdio.h>
#define SNAPSHOT_HEIGHT 240
#define SNAPSHOT_WIDTH 360
#define MAX_TEMP 30
#include <pthread.h>
#include "queue.h"

typedef enum status
{
    CAPTURE_ACTIVE      = 1,
    RECORD_ACTIVE       = 2,
    STOP_RECORD_ACTIVE  = 4,
    SNAPSHOT_ACTIVE     = 8
}status;
typedef struct stage{

    void* (*my_task)(void*);
    pthread_t thread;
    int isActive;
    queue* sourseQu;
    queue* destQu;
    struct stage * next_stage;
  //  struct stage * prev_stage;
    int is_enqueue;
    int is_dequeue;
}stage;

typedef struct snapshot_t{
    char* file_name;  //full path ?
    int width;
    int height;
    char * type;  //GPEG,PNG,ppm
}snapshot_t;

typedef struct handler{
    char * static_mat_rgb[MAX_TEMP*3];    
    stage * stages;
    status my_status;
    FILE *  fp;
    snapshot_t snapshot;

}handler;

typedef  struct task{

    stage * my_stage;
    handler * handle;
    node * my_current_node;

} task;

void run_task(task * my_task);

#endif // HANDLE_STAGES_H
