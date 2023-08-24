

extern "C"
{

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

}
#include "player.h"
#include <QDebug>
#include <stdio.h>
#include<iostream>
using namespace std;
void openMp4File(AVFormatContext *outFmtCtx,AVCodecParameters *codecPara);
int wirteMP4(AVPacket *packet,AVStream *inVStream, AVStream *outVStream , AVFormatContext *outFmtCtx,int outVStreamIndex);
Player::Player():is_start(false),is_pause(false),is_record(false),is_stop(false){}

Player::~Player()
{

}
void Player::setUrl(std::string url){
    rtsp_url = url;
}
void Player::start(){
    std::thread t(this->run,this);
    t.detach();
}
void Player::stop(){
    is_stop = true;
}
void Player::startPlay()
{
    ///调用 QThread 的start函数 将会自动执行下面的run函数 run函数是一个新的线程
    ///
    if(!is_start){
        this->start();
        qDebug()<<"start thread";
    }

    is_start = true;

}

void Player::stopPlay(){
    is_start = false;
}
void Player::pausePlay(){

    is_pause = true;
}


void Player::startRecord(){
    is_record = true;
}
void Player::stopRecord(){
    is_record = false;
}
bool Player::getRecordStatus(){
    return is_record;
}
void Player::run(void* pParam)
{

    Player *ptrPlayerObject = (Player *)pParam;
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    const AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;
    uint8_t *out_buffer;

    static struct SwsContext *img_convert_ctx;

    int videoStream, i, numBytes;
    int ret, got_picture;

    avformat_network_init();   ///初始化FFmpeg网络模块，2017.8.5---lizhen

    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();

    ///2017.8.5---lizhen
    AVDictionary *avdic=NULL;
    char option_key[]="rtsp_transport";
    char option_value[]="udp";
    av_dict_set(&avdic,option_key,option_value,0);
    char option_key2[]="max_delay";
    char option_value2[]="100";
    av_dict_set(&avdic,option_key2,option_value2,0);
    ///rtsp地址，可根据实际情况修改
    char url[]="rtsp://192.168.1.12:8554/123";
//    if(pplayer->rtsp_url.empty()){
//        return;
//    }
//    const char *url = pplayer->rtsp_url.c_str();
    if (avformat_open_input(&pFormatCtx, url, NULL, &avdic) != 0) {
        printf("can't open the file. \n");
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Could't find stream infomation.\n");
        return;
    }


    videoStream = -1;

    ///循环查找视频中包含的流信息，直到找到视频类型的流
    ///便将其记录下来 保存到videoStream变量中
    ///这里我们现在只处理视频流  音频流先不管他
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            printf("video duration :%d",pFormatCtx->streams[i]->duration);
            break;
        }
    }

    ///如果videoStream为-1 说明没有找到视频流
    if (videoStream == -1) {
        printf("Didn't find a video stream.\n");
        return;
    }
    printf("nb_stream:%d videoStream:%d\n",pFormatCtx->nb_streams,videoStream);

    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id); //id = 27

    //save mp4

    const char * outFileName = "save.mp4";

    int outVStreamIndex = -1;


    AVFormatContext *outFmtCtx;
    //=====================输出部分=========================//
    //打开输出文件并填充格式数据
    if(avformat_alloc_output_context2(&outFmtCtx,NULL,NULL,outFileName)<0){
        printf("Cannot alloc output file context.\n");

        return ;

    }



    //打开输出文件并填充数据
    if(avio_open(&outFmtCtx->pb,outFileName,AVIO_FLAG_READ_WRITE)<0){
        printf("output file open failed.\n");
        return;
    }

    //在输出的mp4文件中创建一条视频流
    AVStream *outVStream = avformat_new_stream(outFmtCtx,NULL);
    if(!outVStream){
        printf("Failed allocating output stream.\n");
        return;
    }
    outVStream->time_base.den=25;
    outVStream->time_base.num=1;
    outVStreamIndex=outVStream->index;

    //查找编码器
    const AVCodec *outCodec = avcodec_find_encoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);
    if(outCodec==NULL){
        printf("Cannot find any encoder.\n");
        return;
    }

    //从输入的h264编码器数据复制一份到输出文件的编码器中
    AVCodecContext *outCodecCtx=avcodec_alloc_context3(outCodec);
    AVCodecParameters *outCodecPara = outFmtCtx->streams[outVStream->index]->codecpar;
    if(avcodec_parameters_copy(outCodecPara,pFormatCtx->streams[videoStream]->codecpar)<0){
        printf("Cannot copy codec para.\n");
        return;
    }
    if(avcodec_parameters_to_context(outCodecCtx,outCodecPara)<0){
        printf("Cannot alloc codec ctx from para.\n");
        return;
    }
    outCodecCtx->time_base.den=25;
    outCodecCtx->time_base.num=1;

    //打开输出文件需要的编码器
    if(avcodec_open2(outCodecCtx,outCodec,NULL)<0){
        printf("Cannot open output codec.\n");
        return;
    }

    printf("============Output Information=============>\n");
    av_dump_format(outFmtCtx,0,outFileName,1);
    printf("============Output Information=============<\n");

    // end of open mp4

    pCodecCtx = avcodec_alloc_context3(pCodec);
    //
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);

    printf("pCodecCtx->frame_number:%d\n", pCodecCtx->frame_number);
    printf("pCodecCtx->time_base.num:%d\n", pCodecCtx->time_base.num);
    printf("pCodecCtx->time_base.den:%d\n", pCodecCtx->time_base.den);
    printf("pCodecCtx->bit_rate:%d\n", pCodecCtx->bit_rate);
    ///2017.8.9---lizhen
    // pCodecCtx->bit_rate =0;   //初始化为0
    // pCodecCtx->time_base.num=1;  //下面两行：一秒钟25帧
    // pCodecCtx->time_base.den=10;
    // pCodecCtx->frame_number=1;  //每包一个视频帧

    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return;
    }

    ///打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    ///这里我们改成了 将解码后的YUV数据转换成RGB32
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);

    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width,pCodecCtx->height,1);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    av_image_fill_arrays(
                pFrameRGB->data,
                pFrameRGB->linesize,
                out_buffer,
                AV_PIX_FMT_RGBA,
                pCodecCtx->width,
                pCodecCtx->height,
                1
                );
    int y_size = pCodecCtx->width * pCodecCtx->height;

    packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据

    //SAVE MP4
    // save mp4

    //写入文件头
    if(avformat_write_header(outFmtCtx,NULL)<0){
        printf("Cannot write header to file.\n");
        return ;
    }


    AVStream *inVStream = pFormatCtx->streams[videoStream];

    //enf of save mp4

    while (1)
    {
        if(ptrPlayerObject->is_stop){
            qDebug()<<"playstop";
            break;
        }
        if (av_read_frame(pFormatCtx, packet) < 0)
        {
            if(ptrPlayerObject->getRecordStatus()){
                 av_write_trailer(outFmtCtx);
                 ptrPlayerObject->stopRecord();
            }

             qDebug()<<"readframe over";
            break;
            //continue; //这里认为视频读取完了
        }

        if (packet->stream_index == videoStream) {

            ret = avcodec_send_packet(pCodecCtx,packet);
            if( 0 != ret){
                continue;
            }


            while (avcodec_receive_frame(pCodecCtx,pFrame) == 0){
                sws_scale(img_convert_ctx,
                          (uint8_t const * const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                          pFrameRGB->linesize);
                //把这个RGB数据 用QImage加载
                //QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGBA8888);
                //QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
                ptrPlayerObject->mpCallbackfun((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,ptrPlayerObject->mptr); // 改为回调
                //emit sig_GetOneFrame(image);  //发送信号
                //    ///2017.8.11---lizhen
                //             //提取出图像中的R数据
                //             for(int i=0;i<pCodecCtx->width;i++)
                //             {
                //                 for(int j=0;j<pCodecCtx->height;j++)
                //                 {
                //                     QRgb rgb=image.pixel(i,j);
                //                     int r=qRed(rgb);
                //                     image.setPixel(i,j,qRgb(r,0,0));
                //                 }
                //             }
                // emit sig_GetRFrame(image);


                // mp4

                if(ptrPlayerObject->is_record){
                     wirteMP4(packet,inVStream,outVStream,outFmtCtx,outVStreamIndex);
                }


                //end of mp4
                //msleep(0.02); //停一停  不然放的太快了
                std::this_thread::sleep_for(std::chrono::milliseconds(20)); //修复调帧卡顿
            }


        }
        av_packet_unref(packet); //释放资源,否则内存会一直上升

        ///2017.8.7---lizhen


    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
     qDebug()<<"quit thread";
}
int wirteMP4(AVPacket *packet,AVStream *inVStream, AVStream *outVStream , AVFormatContext *outFmtCtx,int outVStreamIndex){
    // mp4
    //FIXME：No PTS (Example: Raw H.264)
    //Simple Write PTS
    //如果当前处理帧的显示时间戳为0或者没有等等不是正常值
    int frame_index = 0;
    if(packet->pts==AV_NOPTS_VALUE){
        printf("frame_index:%d\n", frame_index);
        //Write PTS
        AVRational time_base1 = inVStream->time_base;
        //Duration between 2 frames (us)
        int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(inVStream->r_frame_rate);
        //Parameters
        packet->pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
        packet->dts = packet->pts;
        packet->duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
        frame_index++;
    }
    //Convert PTS/DTS
    packet->pts = av_rescale_q_rnd(packet->pts, inVStream->time_base, outVStream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet->dts = av_rescale_q_rnd(packet->dts, inVStream->time_base, outVStream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    packet->duration = av_rescale_q(packet->duration, inVStream->time_base, outVStream->time_base);
    packet->pos = -1;
    packet->stream_index = outVStreamIndex;
    printf("Write 1 Packet. size:%5d\tpts:%ld\n", packet->size, packet->pts);
    //Write
    if (av_interleaved_write_frame(outFmtCtx, packet) < 0) {
        printf("Error muxing packet\n");

        //break;
    }
    //av_packet_unref(packet);

    //end of mp4
}


void openMp4File(AVFormatContext *outFmtCtx,AVCodecParameters *codecPara)
{


    const char * outFileName = "save";
    int outVStreamIndex = -1;

    //AVCodecParameters *codecPara = inVFmtCtx->streams[inVStreamIndex]->codecpar;//输入视频流的编码参数


    //=====================输出部分=========================//
    //打开输出文件并填充格式数据

    if(avformat_alloc_output_context2(&outFmtCtx,NULL,NULL,outFileName)<0){
        printf("Cannot alloc output file context.\n");
        return;
    }

    //打开输出文件并填充数据
    if(avio_open(&outFmtCtx->pb,outFileName,AVIO_FLAG_READ_WRITE)<0){
        printf("output file open failed.\n");
        return;
    }

    //在输出的mp4文件中创建一条视频流
    AVStream *outVStream = avformat_new_stream(outFmtCtx,NULL);
    if(!outVStream){
        printf("Failed allocating output stream.\n");
        return;
    }
    outVStream->time_base.den=25;
    outVStream->time_base.num=1;
    outVStreamIndex=outVStream->index;

    //查找编码器
    const AVCodec *outCodec = avcodec_find_encoder(codecPara->codec_id);
    if(outCodec==NULL){
        printf("Cannot find any encoder.\n");
        return;
    }

    //从输入的h264编码器数据复制一份到输出文件的编码器中
    AVCodecContext *outCodecCtx=avcodec_alloc_context3(outCodec);
    AVCodecParameters *outCodecPara = outFmtCtx->streams[outVStream->index]->codecpar;
    if(avcodec_parameters_copy(outCodecPara,codecPara)<0){
        printf("Cannot copy codec para.\n");
        return;
    }
    if(avcodec_parameters_to_context(outCodecCtx,outCodecPara)<0){
        printf("Cannot alloc codec ctx from para.\n");
        return;
    }
    outCodecCtx->time_base.den=25;
    outCodecCtx->time_base.num=1;

    //打开输出文件需要的编码器
    if(avcodec_open2(outCodecCtx,outCodec,NULL)<0){
        printf("Cannot open output codec.\n");
        return;
    }

    printf("============Output Information=============>\n");
    av_dump_format(outFmtCtx,0,outFileName,1);
    printf("============Output Information=============<\n");
}

bool Player::registerCallBack(void (*callfuct)(uchar* ,int,int,void*) ,void *ptr){



    mpCallbackfun = callfuct;
    mptr = ptr;
    return true;
}
