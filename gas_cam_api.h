#ifndef GAS_CAM_API_H
#define GAS_CAM_API_H
#include <stdbool.h>
#include <stdio.h>
#include "handle_stages.h"
#define STAGES_NUMBER 3
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
//typedef struct ppm_image{
//    int width;
//    int height;
//    char *data;
//    size_t size;
//    char * name;
//} ppm_image;
typedef struct {

    void* (*init)();
    void (*free_all)(void* handle);
    int (*start_record)(void* handler);
    int (*stop_record)(void* handler);
    int (*start_streaming)(streaming_t*,char * file_name);
    int (*stop_streamig)(streaming_t*);
    int (*do_snapshot)(void*);
    char* (*get_dll_version)();
    char* (*get_video_statics)(record_t*);
    char* (*get_status)();
}gazapi_t;


#endif // GAS_CAM_API_H
