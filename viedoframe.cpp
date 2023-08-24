#include "viedoframe.h"
#include "ui_viedoframe.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>
#include <iostream>
ViedoFrame::ViedoFrame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViedoFrame)
{
    ui->setupUi(this);
    mPlayer = new Player;
    mPlayer->registerCallBack(callback,(void*)this);
}
void ViedoFrame::callback(uchar* buffer,int width,int height,void* object_ptr){

    ViedoFrame * vfptr = (ViedoFrame*) object_ptr;
    vfptr->transBufferToImage(buffer,width,height);
    vfptr->update();

}
ViedoFrame::~ViedoFrame()
{
    delete ui;
}




void ViedoFrame::slotGetOneFrame(QImage img)
{
    mImage = img;
    update(); //调用update将执行 paintEvent函数
}
///小窗口显示
void ViedoFrame::slotGetRFrame(QImage img)
{
    R_mImage = img;
    update(); //调用update将执行 paintEvent函数
}
void ViedoFrame::paintEvent(QPaintEvent *event)
{



    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成白色

    if (mImage.size().width() <= 0) return;

    ///将图像按比例缩放成和窗口一样大小
    //QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);
    QImage img = mImage.scaled(this->ui->frame->size(),Qt::KeepAspectRatio);
    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

//    qDebug()<<"x==> "<<x<<"y==> "<<y;
//    qDebug()<<"this->width()"<< this->width()<<"this->height() "<<this->height();
//    qDebug()<<"img.width()"<< img.width()<<"img.height() "<<img.height();
    painter.drawImage(QPoint(x,y),img); //画出图像

    if(open_red==true){
        ///2017.8.12
        QWidget *red_video=new QWidget(this);
        red_video->resize(this->width()/3,this->height()/3);
        ///2017.8.11---lizhen
        //提取出图像中的R数据
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, red_video->width(),red_video->height()); //先画成白色

        if (R_mImage.size().width() <= 0) return;

        ///将图像按比例缩放成和窗口一样大小
        QImage R_img = R_mImage.scaled(red_video->size(),Qt::KeepAspectRatio);

        int R_x = red_video->width() - R_img.width();
        int R_y = red_video->height() - R_img.height();

        R_x /= 2;
        R_y /= 2;

        painter.drawImage(QPoint(R_x,R_y),R_img);  //画出图像
    }

    ///2017.8.10---lizhen
    //获取图像中心点
    double x0=this->width()/2;
    double y0=this->height()/2;

    //载体偏移角度，可从设备处获得
    //    double alpha=2;             //横滚角alpha
    //    int length=60;

    //设备偏移后的“水平”参考坐标
    //横滚角产生
    //    double x_Horizental_right=length*qCos(alpha);
    //    double y_Horizental_right=-length*qSin(alpha);
    //    double x_Horizental_left=-length*qCos(alpha);
    //    double y_Horizental_left=length*qSin(alpha);
    //    double x_Vertical_up=length*qSin(alpha);
    //    double y_Vertical_up=length*qCos(alpha);
    //    double x_Vertical_down=-length*qSin(alpha);
    //    double y_Vertical_down=-length*qCos(alpha);

    ///水平参考坐标系，2017.8.7---lizhen
    painter.setPen(QPen(Qt::blue,1,Qt::DotLine));
    painter.drawLine( x0-40,y0, x0+40,y0);
    painter.drawLine( x0,y0-40, x0,y0+40);

    ///横滚运动-偏移坐标系，2017.8.7---lizhen
    // if(alpha!=0)
    // {
    //     painter.setPen(QPen(Qt::blue,3));
    //     painter.drawLine( x0+x_Horizental_left,y0+y_Horizental_left, x0+x_Horizental_right,y0+y_Horizental_right);
    //     painter.drawLine( x0+x_Vertical_up,y0+y_Vertical_up, x0+x_Vertical_down,y0+y_Vertical_down);
    // }
}

void ViedoFrame::on_pushButton_play_clicked()
{
    is_play = !is_play;
    if(is_play){
        mPlayer->setUrl(ui->lineEdit_url->text().toStdString());
        mPlayer->startPlay();

        ui->pushButton_play->setText("playing");
    }
    else{
        mPlayer->stopPlay();
        ui->pushButton_play->setText("play");
    }


}


void ViedoFrame::on_pushButton_record_clicked()
{


    is_recording = !is_recording;
    if(is_play){
        mPlayer->startRecord();
        ui->pushButton_play->setText("recording");
    }
    else{
        ui->pushButton_play->setText("record");
        mPlayer->stopRecord();
    }

}


void ViedoFrame::transBufferToImage(const uchar *buff,int width,int height,int fotmat){

    if(!buff){
        NULL;
    }
    QImage tmpImg(buff,width,height,QImage::Format_RGBA8888);
    //QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
    mImage = tmpImg.copy();

}
