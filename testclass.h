#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>
#include <QtTest/QtTest>
#include "player.h"
class TestClass : public QObject
{
    Q_OBJECT
public:
    explicit TestClass(QObject *parent = nullptr);
private:
    Player *play_obj;
private slots:
    void initTestCase();
    void cleanupTestCase();

    void case_setUrl();
    void case_startPlay();
    void case_stopPlay();
signals:

};

#endif // TESTCLASS_H
