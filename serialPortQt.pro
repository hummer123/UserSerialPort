#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T15:12:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialPortQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
            qextserial/qextserialport.cpp \
    receivethread.cpp \
    ringbuff.cpp \
    paintwave.cpp

HEADERS  += mainwindow.h\
        qextserial/qextserialport_global.h \
        qextserial/qextserialport.h \
    receivethread.h \
    ringbuff.h \
    paintwave.h

win32{
    SOURCES += qextserial/qextserialport_win.cpp
}

unix{
    SOURCES += qextserial/qextserialport_unix.cpp
}

RC_ICONS += ./images/serialApp.ico

RESOURCES += \
    res.qrc


