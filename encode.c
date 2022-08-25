#include "encode.h"
#include <sys/time.h>
#include <assert.h>
#include "save_snapshot.h"

static int start_time=0;
AVFrame *frame;
encoder_t *p_encoder;
uint8_t ft_endcode[] = { 0, 0, 1, 0xb7 };
FILE *outline;

//function to save all the frames that added to the video
int ff_encoder(encoder_t *p_encoder,FILE * outfile){

    int ret=0;
    ret = avcodec_send_frame(p_encoder->enc_ctx, p_encoder->frame);
    assert(ret >= 0);
    while (ret >= 0) {
        ret = avcodec_receive_packet(p_encoder->enc_ctx, p_encoder->pkt);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return 0;
        assert(ret >= 0) ;
        fwrite(p_encoder->pkt->data, 1, p_encoder->pkt->size, outfile);
        av_packet_unref(p_encoder->pkt);


    }

}
uint64_t GetMHClock(void){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)tv.tv_sec * 1000000L + (uint64_t)tv.tv_usec;
}

encoder_t * init_encoder(int w,int h,int gop){
    const AVCodec *codec;
    int ret=0;
    encoder_t *p_encoder =(encoder_t*) calloc (1,sizeof (encoder_t)) ;
    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
    assert(codec);
    p_encoder->frame=av_frame_alloc();
    assert(p_encoder->frame);

    p_encoder->frame->width=w;
    p_encoder->frame->height=h;
    p_encoder->frame->format=AV_PIX_FMT_YUV420P;
    ret = av_frame_get_buffer(p_encoder->frame, 0);
    assert(ret>=0);

    p_encoder->enc_ctx=avcodec_alloc_context3(codec);
    assert(p_encoder->enc_ctx) ;

    p_encoder->pkt=av_packet_alloc();
    assert(p_encoder->pkt);
    p_encoder->enc_ctx->bit_rate=400000;
    p_encoder->enc_ctx->width=w;
    p_encoder->enc_ctx->height=h;

    p_encoder->enc_ctx->time_base = (AVRational){1, 25};
    p_encoder->enc_ctx->framerate = (AVRational){25, 1};
    p_encoder->enc_ctx->gop_size = gop;
    p_encoder->enc_ctx->max_b_frames = 1;
    p_encoder->enc_ctx->pix_fmt=AV_PIX_FMT_YUV420P;

    ret=avcodec_open2(p_encoder->enc_ctx, codec, NULL);
    assert(ret>=0);
    return (p_encoder);

}

//function to encode pictures into a video

void* encode(task * task)
{
    stage * my_stage=task->my_stage;
    handler * my_handler=task->handle;
    int ret=0;
    if(task->my_current_node->data!=NULL)
    {
        my_stage->isActive=1;
        YUV * yuv=(YUV *)task->my_current_node->data;

        printf("-----encode\n-----%c",yuv->u[0]);
        if(start_time==0)//if now it is th efirst frame
        {
            fflush(stdout);
            p_encoder=init_encoder(my_handler->record.width,my_handler->record.height,12);
            frame=p_encoder->frame;
            outline=fopen("t.ts","wb");
            assert(outline);
        }
        //        ret = av_frame_make_writable(frame);
        //        frame->data[0][frame->linesize[0]]=yuv->y;
        //        frame->data [1][frame->linesize[1]]=yuv->u;
        //        frame->data [2][frame->linesize[2]]=yuv->v;


        ret = av_frame_make_writable(frame);
        for (int i=0;i<frame->height;i++)
            for (int j=0;j<frame->width;j++)
                frame->data[0][i*frame->linesize[0]+j]=yuv->y[i*frame->width+j/2];
        for(int i=0;i<p_encoder->enc_ctx->height/2;i++)
            for(int j=0;j<p_encoder->enc_ctx->width/2;j++){
                frame->data [1][i*frame->linesize[1]+j]=yuv->u[(i/2)*frame->width+j/2];
                frame->data [2][i*frame->linesize[2]+j]= yuv->u[(i/2)*frame->width+j/2];
            }

        if (start_time==0){
            frame->pts=0;
            start_time=GetMHClock();
        }
        else
            frame->pts = GetMHClock()-start_time;
        ff_encoder(p_encoder,outline);
        //task->my_current_node=createNode(yuv,sizeof(YUV*));
        freeNode(task->my_current_node);

    }
    else
    {
        my_stage->isActive=0;
        printf("encode_finish");
        avcodec_free_context(&p_encoder->enc_ctx);
        av_frame_free(&frame);
        av_packet_free(&p_encoder->pkt);
        fwrite(ft_endcode, 1, sizeof(ft_endcode), outline);
        fclose(outline);

    }


}
