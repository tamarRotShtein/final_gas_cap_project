#include "encode.h"
#include <sys/time.h>

static int start_time=0;
AVFrame *frame;
AVPacket *pkt;
AVCodecContext *c= NULL;
uint8_t endcode[] = { 0, 0, 1, 0xb7 };
FILE *f;

//function to save all the frames that added to the video
void my_encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
               FILE *outfile)
{
    int ret;
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        printf("Write packet %d (size=%5d) flag=%d \n", pkt->pts, pkt->size,pkt->flags);

        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}
uint64_t GetMHClock(void){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)tv.tv_sec * 1000000L + (uint64_t)tv.tv_usec;
}

//function to encode pictures into a video

void* encode(task * task)
{
    stage * my_stage=task->my_stage;
    if(task->my_current_node->data!=NULL)
    {
        my_stage->isActive=1;
        YUV * yuv=(YUV *)task->my_current_node->data;
        printf("-----encode\n-----%c",yuv->u[0]);
        if(start_time==0)
        {
            const char *filename={"f.ts"}, *codec_name;
            const AVCodec *codec;

            int ret;
            codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
            if (!codec) {
                fprintf(stderr, "Codec '%s' not found\n", codec_name);
                exit(1);
            }
            c = avcodec_alloc_context3(codec);
            if (!c) {
                fprintf(stderr, "Could not allocate video codec context\n");
                exit(1);

            }
            pkt = av_packet_alloc();
            if (!pkt)
                exit(1);
            /* put sample parameters */
            c->bit_rate = 400000;
            /* resolution must be a multiple of two */
            c->width = SNAPSHOT_WIDTH;
            c->height = SNAPSHOT_HEIGHT;
            /* frames per second */
            c->time_base = (AVRational){1, 25};
            c->framerate = (AVRational){25, 1};
            c->gop_size = 5;
            c->max_b_frames = 1;
            c->pix_fmt=AV_PIX_FMT_YUV420P;
            ret = avcodec_open2(c, codec, NULL);
            if (ret < 0) {
                exit(1);
            }
            f = fopen(filename, "wb");
            if (!f) {
                fprintf(stderr, "Could not open %s\n", filename);
                exit(1);
            }
            frame = av_frame_alloc();
            if (!frame) {
                fprintf(stderr, "Could not allocate video frame\n");
                exit(1);
            }
            frame->format = c->pix_fmt;
            frame->width  = c->width;
            frame->height = c->height;
            ret = av_frame_get_buffer(frame, 0);
            if (ret < 0) {
                fprintf(stderr, "Could not allocate the video frame data\n");
                exit(1);
            }

            ret = av_frame_make_writable(frame);
            if (ret < 0)
                exit(1);
        }

        frame->data[0][frame->linesize[0]]=(uint8_t)yuv->y;
        frame->data [1][frame->linesize[1]]=(uint8_t)yuv->u;
        frame->data [2][frame->linesize[2]]=(uint8_t) yuv->v;
        if (start_time==0){
            frame->pts=0;
            start_time=GetMHClock();
        }
        else
            frame->pts = GetMHClock()-start_time;
        my_encode(c, frame, pkt, f);
        task->my_current_node=createNode(yuv,sizeof(YUV*));

    }
    else
    {
        my_stage->isActive=0;
        printf("encode_finish");
        avcodec_free_context(&c);
        av_frame_free(&frame);
        av_packet_free(&pkt);

    }


}
