#ifndef ENCODE_H
#define ENCODE_H
#include "handle_stages.h"
#include "yuv_convertor.h"
#include <libavcodec/avcodec.h>

typedef struct
{
    AVCodecContext *enc_ctx;
    AVFrame *frame;
    AVPacket *pkt;
}encoder_t;


//function to encode pictures into a video
void* encode(task * task);



#endif // ENCODE_H
