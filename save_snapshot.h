#ifndef SAVE_SNAPSHOT_H
#define SAVE_SNAPSHOT_H
#include <stdio.h>
#include "handle_stages.h"
#include <setjmp.h>
#include <jpeglib.h>

//struct for function in bmp type
typedef struct ppm_image{
    int width;
    int height;
    char *data;
    size_t size;
    char * name;
} ppm_image;

//function to save a picture by type
void save_snapshot(char* ,snapshot_t );

#endif // SAVE_SNAPSHOT_H
