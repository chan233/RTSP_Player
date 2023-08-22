#ifndef VIEDOFRAME_H
#define VIEDOFRAME_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
namespace Ui {
class ViedoFrame;
}

class ViedoFrame : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViedoFrame(QWidget *parent = nullptr);
    ~ViedoFrame();
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void slotGetOneFrame(QImage img);
    void slotGetRFrame(QImage img);///2017.8.11---lizhen
private:
    Ui::ViedoFrame *ui;
    QImage mImage; //记录当前的图像
    QImage R_mImage; //2017.8.11---lizhen
     bool open_red=false;
};

#endif // VIEDOFRAME_H
