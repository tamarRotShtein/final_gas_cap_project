#ifndef SNAPSHOT_T_H
#define SNAPSHOT_T_H

typedef struct snapshot_t{
    char* file_name;  //full path ?
    int width;
    int height;
    char * type;  //GPEG,PNG,ppm
}snapshot_t;

#endif // SNAPSHOT_T_H
