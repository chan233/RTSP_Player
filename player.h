/**
 * 李震
 * 我的码云：https://git.oschina.net/git-lizhen
 * 我的CSDN博客：http://blog.csdn.net/weixin_38215395
 * 联系：QQ1039953685
 */

#ifndef Player_H
#define Player_H


#include <string>
#include <thread>
#include <chrono>

typedef unsigned char uchar ;
typedef void (*MyCallBackFunc)(uchar* ,int,int,void*);
class Player
{


public:
    explicit Player();

    ~Player();

    void startPlay();
    void pausePlay();
    void stopPlay();
    void setUrl(std::string url);
    void startRecord();
    void stopRecord();
    bool getRecordStatus();
    void stop();
    bool registerCallBack(void (*callfuct)(uchar*,int,int,void*),void*ptr);


protected:
    static void run(void* pParam);

private:
    void start();
    bool is_start;
    bool is_pause ;
    bool is_record ;
    bool is_stop;
    std::string rtsp_url;
    MyCallBackFunc mpCallbackfun;
    void *mptr;

};

#endif // Player_H
