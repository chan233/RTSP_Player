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
#include <memory>

typedef unsigned char uchar ;
typedef void (*MyCallBackFunc)(uchar* ,int,int,void*);
class Player
{


public:
    explicit Player();

    ~Player();

    bool startPlay();
    void stopPlay();
    bool getPlayStatus();
    void pausePlay();

    void startRecord();
    void stopRecord();
    bool getRecordStatus();

    void setUrl(std::string url);
    std::string getUrl();

    bool registerCallBack(void (*callfuct)(uchar*,int,int,void*),void*ptr);


protected:
    static void run(void* pParam);
static void threadLog( std::string logmessage);
private:
    void startStreamThread();
    bool _isStart;
    bool _isPause ;
    bool _isRecord ;
    bool _isStop;
    std::string _rtsp_url;
    MyCallBackFunc _ptrCallbackfun;
    void *_ptrParent;


};

#endif // Player_H
