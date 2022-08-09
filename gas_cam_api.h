#ifndef GAS_CAM_API_H
#define GAS_CAM_API_H
#include <stdbool.h>
#include <stdio.h>
#include "handle_stages.h"

#define STAGES_NUMBER 4
#define CAPACITY 10


typedef struct streaming_t{
    int ip;
    int port;
}streaming_t;



typedef struct gpio_t{
    int num;
    int dir;  //in ,out
    int value;  //1 ,0
    int is_blink;
    int operation;  //record,straming
}gpio_t;

typedef struct record_t{
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_t;

typedef struct {

    void* (*init)();//function to init the handler to the record and snapshot
    void (*free_all)(void* handle);//function to free the handler
    int (*start_record)(void* handler);//function to start the record
    int (*stop_record)(void* handler);//function to stop the record
    int (*start_streaming)(streaming_t*,char * file_name);
    int (*stop_streamig)(streaming_t*);
    int (*do_snapshot)(void*);//function to create a picture
    char* (*get_dll_version)();
    char* (*get_video_statics)(record_t*);
    char* (*get_status)();
}gazapi_t;


#endif // GAS_CAM_API_H
