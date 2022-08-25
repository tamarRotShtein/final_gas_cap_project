#ifndef HANDLE_STAGES_H
#define HANDLE_STAGES_H
#include <stdio.h>
#define SNAPSHOT_HEIGHT 240
#define SNAPSHOT_WIDTH 360
#define MAX_TEMP 30
#include <pthread.h>
#include "queue.h"
#include "stage.h"
#include "snapshot_t.h"
#include "record_t.h"

typedef enum status
{
    CAPTURE_ACTIVE      = 1,
    RECORD_ACTIVE       = 2,
    STOP_RECORD_ACTIVE  = 4,
    SNAPSHOT_ACTIVE     = 8
}status;



//struct to the handler of the pipeline
typedef struct handler{

    char * static_mat_rgb[MAX_TEMP*3];//enum to save the status of the handler

    stage * stages;
    status my_status;
    FILE *  fp;
    snapshot_t snapshot;
    record_t record;


}handler;

//struct for the functions of the stages
typedef  struct task{

    stage * my_stage;
    handler * handle;
    node * my_current_node;

} task;

//function to run a task of stage in the pipeline
void run_task(task * my_task);

#endif // HANDLE_STAGES_H
