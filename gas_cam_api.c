#include "gas_cam_api.h"
#include <stdlib.h>
#include "capture.h"
#include "rgb_converter.h"
#include "queue.h"
#include "yuv_convertor.h"
#include <unistd.h>
#include "encode.h"

//function to init the handler to the record and snapshot
void* GAS_API_init(){
    handler* handle=(handler*)malloc(sizeof(handler));
    if(handle==NULL){
        printf("error mallocing\n");
        exit(0);
    }
    //init stages
    int i;
    stage *new_stage=NULL;
    new_stage=(stage * )malloc(sizeof(stage));
    new_stage->is_dequeue=0;
    new_stage->is_enqueue=1;
    handle->stages=new_stage;
    stage * temp=handle->stages;
    for(i=0;i<STAGES_NUMBER-1;i++)
    {
        new_stage=(stage * )malloc(sizeof(stage));
        new_stage->is_dequeue=1;
        new_stage->is_enqueue=1;
        temp->next_stage=new_stage;
        // new_stage.prev_stage=temp;
        temp=temp->next_stage;
    }
    temp->next_stage=NULL;
    temp->is_enqueue=0;
    //init queues
    temp =handle->stages;
    while(temp)
    {
        if(temp->is_enqueue)
        {
            temp->destQu=createQueue(CAPACITY);
            temp->next_stage->sourseQu=temp->destQu;
        }
        temp=temp->next_stage;
    }
    //init functions
    handle->stages[0].my_task=capture;
    handle->stages->next_stage->my_task=rgb_converter;
    temp=handle->stages->next_stage->next_stage;
    temp->my_task=yuv_converter;
    temp=temp->next_stage;
    temp->my_task=encode;


    //init action
    handle->my_status=0;
    int j=0;
    char z=(char)255,x=0;
    while(j!=MAX_TEMP*3/2)
        {
            //rand numbers
            handle->static_mat_rgb[j++]=x;
            handle->static_mat_rgb[j++]=x;
            handle->static_mat_rgb[j++]=z;
        }
    while(j!=MAX_TEMP*3)
        {
            //rand numbers
            handle->static_mat_rgb[j++]=z;
            handle->static_mat_rgb[j++]=x;
            handle->static_mat_rgb[j++]=x;
        }

        for ( i=0;i<80;i++) {
            handle->static_mat_rgb[i++]=x;
            handle->static_mat_rgb[i++]=x;
            handle->static_mat_rgb[i]=z;
           }
//    handle->static_mat_rgb[9]=y;
//    handle->static_mat_rgb[10]=x;
//    handle->static_mat_rgb[11]=z;
//    handle->static_mat_rgb[81]=x;
//    handle->static_mat_rgb[82]=z;
//    handle->static_mat_rgb[83]=x;
    return (void*)handle;
}

//function to free the handler
void GAS_API_free_all(void* handle){
    handler* my_handler=(handler*)(handle);
    stage * temp=my_handler->stages;
    while(temp)
    {
        stage * to_free=temp;
        if(temp->is_enqueue)
            freeQueue(temp->destQu);
        temp=temp->next_stage;
        free(to_free);
    }
    free(my_handler);
}
//function to stop the record
int GAS_API_stop_record(void* handle)
{
    printf ("====stop_record!====\n");
    handler* my_handler=(handler*)(handle);
    my_handler->my_status=0;
    stage * temp=my_handler->stages;
    while(temp)
    {
        pthread_join(temp->thread,NULL);
        temp=temp->next_stage;
    }
   // GAS_API_free_all(my_handler);

}
//function to start the record
int GAS_API_start_record(void* handle){
    printf ("====start_record!====\n");
    handler* my_handler=(handler*)(handle);
    my_handler->my_status|=RECORD_ACTIVE;
    my_handler->record.file_name="record";
    my_handler->record.width=SNAPSHOT_WIDTH;
    my_handler->record.height=SNAPSHOT_HEIGHT;
    stage * temp=my_handler->stages;
    task * my_task=NULL;
    while(temp){
        my_task=(task*)malloc(sizeof (my_task));
        my_task->handle=my_handler;
        my_task->my_stage=temp;
        my_task->my_current_node=NULL;
        pthread_create(&(temp->thread),NULL,run_task,my_task);
        temp=temp->next_stage;
    }

    return 0;

}
//function to create a picture
int GAS_API_do_snapshot(void * handle){

    printf("GAZ_API_do_snapshot");
    handler* my_handler=(handler*)(handle);
    my_handler->snapshot.type="bmp";
    my_handler->snapshot.width=SNAPSHOT_WIDTH;
    my_handler->snapshot.height=SNAPSHOT_HEIGHT;
    //    my_handler->snapshot.file_name=fprintf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    my_handler->snapshot.file_name="1";
    my_handler->my_status|=SNAPSHOT_ACTIVE;
    if(!(my_handler->my_status & RECORD_ACTIVE))
    {

        task * my_task=NULL;
        stage * temp=my_handler->stages;
        int i=0;
        while(i<2 && temp)
        {
            my_task=(task*)malloc(sizeof (my_task));
            my_task->handle=my_handler;
            my_task->my_stage=temp;
            my_task->my_current_node=NULL;
            pthread_create(&(temp->thread),NULL,run_task,my_task);
            i++;
            temp=temp->next_stage;
        }
        temp=my_handler->stages;
        while(i<2 && temp)
        {
            pthread_join(temp->thread,NULL);
            i++;
            temp=temp->next_stage;
        }
    }
    return 1;
}
gasapi_t p_gaz_api= {
    .init=GAS_API_init,
    .free_all=GAS_API_free_all,
    .start_record=GAS_API_start_record,
    .stop_record=GAS_API_stop_record,
    //    .start_streaming=GAS_API_start_streaming,
    //    .stop_streamig=GAS_API_stop_streamig,
    .do_snapshot=GAS_API_do_snapshot,
    //    .get_dll_version=GAS_API_get_dll_version,
    //    .get_video_statics=GAS_API_get_video_statics,
    //    .get_status=GAS_API_get_status
};
