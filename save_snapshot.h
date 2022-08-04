#ifndef SAVE_SNAPSHOT_H
#define SAVE_SNAPSHOT_H
#include <stdio.h>
#include "handle_stages.h"
#include <setjmp.h>
#include <jpeglib.h>
typedef struct ppm_image{
    int width;
    int height;
    char *data;
    size_t size;
    char * name;
} ppm_image;

void save_snapshot(char* ,snapshot_t );

#endif // SAVE_SNAPSHOT_H
