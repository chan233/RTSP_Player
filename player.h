/**
 * 李震
 * 我的码云：https://git.oschina.net/git-lizhen
 * 我的CSDN博客：http://blog.csdn.net/weixin_38215395
 * 联系：QQ1039953685
 */

#ifndef Player_H
#define Player_H

#include <QThread>
#include <QImage>
#include <string>
//2017.8.10---lizhen
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
typedef void (*MyCallBackFunc)(uchar* ,int,int,void*);
class Player : public QThread
{
    Q_OBJECT

public:
    explicit Player();

    ~Player();

    void startPlay();
    void pausePlay();
    void stopPlay();
    void setUrl(std::string url);
    void startRecord();
    void stopRecord();
    bool registerCallBack(void (*callfuct)(uchar*,int,int,void*),void*ptr);

signals:
    void sig_GetOneFrame(QImage); //每获取到一帧图像 就发送此信号
    void sig_GetRFrame(QImage);   ///2017.8.11---lizhen

protected:
    void run();

private:
    QString mFileName;
    MyCallBackFunc mpCallbackfun;
    void *mptr;
    //2017.8.10---lizhen
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    bool is_start;
    bool is_pause ;
    bool is_record ;
    std::string rtsp_url;

};

#endif // Player_H
