#include "receivethread.h"
#include <QDebug>

#define MAXDATASIZE 1024

ReceiveThread::ReceiveThread(QextSerialPort *inCom, QObject *parent) : QThread(parent)
{
    runCom = inCom;
    popBuf = new char[MAXDATASIZE];
    dataBuff = new RingBuff;
    isThreadStop = false;
    dataOk = false;
    outTime = 0;
    qDebug() << "serialThread Beging";
}

ReceiveThread::~ReceiveThread()
{
    runCom = NULL;
    delete popBuf;
    delete dataBuff;

    qDebug() << "serialThread destructor...";
}


void ReceiveThread::run()
{
    while(1)
    {
        //QMutexLocker locker(&mutex);
        mutex.lock();
        if(isThreadStop == true)
        {
            mutex.unlock();
            return;
        }
        mutex.unlock();

        if(runCom->bytesAvailable() > 0)
        {
            recData = runCom->readAll();
            dataBuff->pushData(recData);
            //qDebug() << "Run Rec" << recData;
        }

        dataOk = dataBuff->popData(popBuf);
        if(dataOk)
        {
            emit serialFinished(popBuf);
        }

        msleep(50);
    }
}

void ReceiveThread::stopThread()
{
    //QMutexLocker locker(&mutex);
    mutex.lock();
    isThreadStop = true;
    mutex.unlock();
    this->wait();

    qDebug() << "serialThread End";
}
