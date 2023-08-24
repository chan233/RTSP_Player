#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{

}

void TestClass::initTestCase(){
    play_obj = new Player;
    QVERIFY(play_obj != NULL);
}

void TestClass::cleanupTestCase(){
    delete play_obj;
    play_obj = nullptr;

}


void TestClass::case_setUrl(){
    std::string url = "rtsp:://127.0.0.1:8554/123";
    play_obj->setUrl(url);
    QVERIFY(play_obj->getUrl() == url);

}

void TestClass::case_startPlay(){
    play_obj->setUrl("");
    play_obj->startPlay();
    QVERIFY(play_obj->getPlayStatus() == false);
}
void TestClass::case_stopPlay(){
    play_obj->stopPlay();
    QVERIFY(play_obj->getPlayStatus() == false);
}
