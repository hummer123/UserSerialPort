#ifndef RINGBUFF_H
#define RINGBUFF_H

#include <QByteArray>
#include <QObject>

#define BUFF_SIZE   4096

class RingBuff : public QObject
{
public:
    RingBuff();
    virtual ~RingBuff();

public:
    bool pushData(QByteArray );
    bool popData(char *);
    bool isEmpty();
    bool isFull();
    void setEmpty();
    int getUseSize();
    unsigned int getBuffSize();

private:
    char *ringBuf;
    unsigned int rheadPos;
    unsigned int rtailPos;
};

#endif // RINGBUFF_H
