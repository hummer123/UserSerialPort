#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
//#include <QMutexLocker>
#include "qextserial/qextserialport.h"
#include "ringbuff.h"


class ReceiveThread : public QThread
{
    Q_OBJECT

public:
    ReceiveThread(QextSerialPort *, QObject *parent = 0);
    ~ReceiveThread();

protected:
    void run();

public:
    void stopThread();

signals:
    void serialFinished(const char *);

private:
    QMutex mutex;
    QextSerialPort *runCom;
    QByteArray recData;
    RingBuff *dataBuff;
    char *popBuf;
    volatile bool isThreadStop;
    bool dataOk;
    unsigned char outTime = 0;
};

#endif // RECEIVETHREAD_H
