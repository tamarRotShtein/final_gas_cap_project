#include "capture.h"
#include "queue.h"
#include<stdlib.h>
#include <unistd.h>
//function to rand matrix for the pixels of the picture
void randMat(int** matrix_temp){
    int i,j,new_temp,height_pos,width_pos,num_iterations=0.1*SNAPSHOT_WIDTH*SNAPSHOT_HEIGHT;
    for(i=0;i<SNAPSHOT_HEIGHT;i++){
        for(j=0;j<SNAPSHOT_WIDTH;j++){
            matrix_temp[i][j]=27;
        }
    }

    for(i=0;i<num_iterations;i++)
    {
        height_pos=rand() % SNAPSHOT_HEIGHT;
        width_pos=rand() % SNAPSHOT_WIDTH;
        new_temp=rand()%(MAX_TEMP+1);
        matrix_temp[height_pos][width_pos]=new_temp;
    }
}

//function to create a single picture

void * capture(task * task){

    handler * my_handler=task->handle;
    if(((my_handler->my_status) & RECORD_ACTIVE) || ((my_handler->my_status) & SNAPSHOT_ACTIVE)){
    printf("-----capture\n-----");
    handler * my_handler=task->handle;
    stage* my_stage=task->my_stage;

    my_stage->isActive=1;
    int i=0,** matrix=NULL;
    matrix=(int**)malloc(sizeof(int *)*SNAPSHOT_HEIGHT);
    for( i=0;i<SNAPSHOT_HEIGHT;i++)  {
        matrix[i]=NULL;
        matrix[i]=(int *)malloc(sizeof(int)*SNAPSHOT_WIDTH);
    }
    randMat(matrix);
    task->my_current_node=createNode(matrix);
    printf("capture %d\n",((int**)(task->my_current_node->data))[0][0]);
    usleep(4000);
    if((my_handler->my_status & STOP_RECORD_ACTIVE))
        my_stage->isActive=0;
    if((my_handler->my_status & SNAPSHOT_ACTIVE) && !(my_handler->my_status & RECORD_ACTIVE))
          my_stage->isActive=0;
    }
    else
    {
        printf("record finish");
        task->my_stage->isActive=0;
        task->my_current_node->data=NULL;
    }
}
