#ifndef YUV_CONVERTOR_H
#define YUV_CONVERTOR_H
#include "handle_stages.h"

//struct to picture in yuv format
typedef struct yuv{
    char y[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH];
    char u[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    char v[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
}YUV;

//function to convert picture from rgb to yuv format
void* yuv_converter(task * task);

#endif // YUV_CONVERTOR_H
