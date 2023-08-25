#ifndef VIEDOFRAME_H
#define VIEDOFRAME_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include "player.h"
namespace Ui {
class ViedoFrame;
}

class ViedoFrame : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViedoFrame(QWidget *parent = nullptr);
    ~ViedoFrame();
    static void callback(uchar* ,int,int,void* object_ptr);
protected:
    void paintEvent(QPaintEvent *event);
private slots:

    void on_pushButton_play_clicked();

    void on_pushButton_record_clicked();


signals:
    void sig_GetOneFrame(bool); //每获取到一帧图像 就发送此信号

private:
    Ui::ViedoFrame *ui;
    QImage mImage; //记录当前的图像
    QImage R_mImage; //2017.8.11---lizhen
    bool open_red=false;
    bool is_play = false;
    bool is_recording = false;
    void transBufferToImage(const uchar *buff,int width,int height,int fotmat = QImage::Format_RGBA8888);

    std::shared_ptr<Player> mPlayer;
};

#endif // VIEDOFRAME_H
