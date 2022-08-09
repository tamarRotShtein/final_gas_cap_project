#include "yuv_convertor.h"
#include<stdlib.h>

void convert_to_yuv(char rgb_matrix[],  YUV *yuv){
    char R,G,B;
    for(int i=0;i<SNAPSHOT_HEIGHT;i++)
    {
        for(int j=0;j<SNAPSHOT_WIDTH;j++)
        {
            R=*(rgb_matrix+i*SNAPSHOT_WIDTH*3+j*3);
            G=*(rgb_matrix+i*SNAPSHOT_WIDTH*3+j*3+1);
            B=*(rgb_matrix+i*SNAPSHOT_WIDTH*3+j*3+2);
            yuv->y[SNAPSHOT_WIDTH*i+j]= (0.257 * R) + (0.504 * G) + (0.098 * B) + 16+'0';
            if(i%2==0&&j%2==0)
            {
                yuv->u[(SNAPSHOT_WIDTH*i+j)/4]= (0.439 * R) - (0.368 * G) - (0.071 * B) + 128+'0';
                yuv->v[(SNAPSHOT_WIDTH*i+j)/4]= -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128+'0';
            }
        }
    }

}
void free_rgb_matrix1(char * m){
    free(m);
    m=NULL;
}

//function to convert picture from rgb to yuv format
void* yuv_converter(task * task)
{
    stage * my_stage=task->my_stage;
    if(task->my_current_node->data!=NULL)
    {
        printf("-----yuv_converter\n-----");

        my_stage->isActive=1;
        node * my_node=task->my_current_node;
        char x=*((char *)my_node->data);
        YUV * yuv=(YUV*)malloc(sizeof(YUV));
        convert_to_yuv((char*)my_node->data,yuv);
        char * c=(char*)(my_node->data);
        free_rgb_matrix1(c);
        freeNode(my_node);
        task->my_current_node=createNode(yuv,sizeof(YUV*));
    }
    else
    {
        my_stage->isActive=0;
        printf("yuv_finish");
    }


}
