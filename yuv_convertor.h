#ifndef YUV_CONVERTOR_H
#define YUV_CONVERTOR_H
#include "handle_stages.h"

typedef struct yuv{
    char y[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH];
    char u[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    char v[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
}YUV;

void* yuv_converter(task * task);

#endif // YUV_CONVERTOR_H
