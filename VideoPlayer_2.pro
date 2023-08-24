#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer_2
TEMPLATE = app


SOURCES += main.cpp \
    player.cpp \
    testclass.cpp \
    videoplayer.cpp \
    mainwindow.cpp \
    viedoframe.cpp

HEADERS  += \
    player.h \
    testclass.h \
    videoplayer.h \
    mainwindow.h \
    viedoframe.h

FORMS    += \
    mainwindow.ui \
    viedoframe.ui

INCLUDEPATH += /opt/ffmpeg/include


LIBS += -L"/opt/ffmpeg/lib/"
LIBS += -lavutil \
	-lavcodec \
       -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lswresample \
        -lswscale
