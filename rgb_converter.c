#include "rgb_converter.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "save_snapshot.h"

void convert_to_rgb(handler * handle,char rgb_matrix[],int** matrix)
{

    int k=0;
    for(int i=0;i<SNAPSHOT_HEIGHT;i++)
        for(int j=0;j<SNAPSHOT_WIDTH;j++)
        {
            int t=matrix[i][j]*3;
            rgb_matrix[k++]=handle->static_mat_rgb[t];
            rgb_matrix[k++]=handle->static_mat_rgb[t+1];
            rgb_matrix[k++]=handle->static_mat_rgb[t+2];
        }
}
void free_matrix(int** m){
    for(int i=0;i<SNAPSHOT_HEIGHT;i++){
        free(m[i]);
        m[i]=NULL;
    }
    m=NULL;
}


void free_rgb_matrix(char * m){
    free(m);
    m=NULL;
}

// function to create matrix of pixels in rgb
void* rgb_converter(task * task){
    stage * my_stage=task->my_stage;
    if(task->my_current_node->data!=NULL){
        printf("-----rgb_converter\n-----");
        handler * my_handler=task->handle;
        char *rgb_matrix=NULL;
        node * my_node=task->my_current_node;
        my_stage->isActive=1;
        rgb_matrix=(char *)malloc(sizeof(char)*(SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH*3));
        *rgb_matrix=0;
        convert_to_rgb(my_handler,rgb_matrix,(int**)my_node->data);
        free_matrix((int**)(my_node->data));
        freeNode(my_node);
        if((my_handler->my_status & SNAPSHOT_ACTIVE))
        {
            save_snapshot(rgb_matrix,my_handler->snapshot);
            my_handler->my_status=my_handler->my_status & (~SNAPSHOT_ACTIVE);
            //free_rgb_matrix(rgb_matrix);
        }
        task->my_current_node=createNode(rgb_matrix,sizeof(rgb_matrix));
        printf("rgb \n");
    }
    else
    {
        my_stage->isActive=0;
        printf("rgb_finish");
    }
}

