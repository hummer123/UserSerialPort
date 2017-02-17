#include "ringbuff.h"
#include <QDebug>
/*
 * 队列长 BUFF_SIZE
 * 环形队列队空 tail = head
 * 环形队列队满 tail = head + 1
 * 整个BUFF有一个空间没用
*/


RingBuff::RingBuff()
{
    ringBuf = new char[BUFF_SIZE];
    qDebug() << "ringbuff.cpp---Creaator ringBuf";
    rheadPos = 0;
    rtailPos = 0;
}

RingBuff::~RingBuff()
{
    delete ringBuf;
    qDebug() << "ringbuff.cpp---Delete ringBuf";
}

bool RingBuff::pushData(QByteArray wData)
{
    const char *sdata = wData.constData();
    int sdataCount = wData.size();

    if(sdataCount == 0)
        return false;
    if(sdataCount > getUseSize())
        return false;
    if(rheadPos >= rtailPos)
    {
        int headCount = BUFF_SIZE - rheadPos;
        if(headCount > sdataCount)
        {
            memcpy(ringBuf+rheadPos, sdata, sdataCount);
            rheadPos += sdataCount;

            return true;
        }
        else
        {
            memcpy(ringBuf+rheadPos, sdata, headCount);
            rheadPos = sdataCount - headCount;
            if(rheadPos > 0)
                memcpy(ringBuf, sdata+headCount, rheadPos);

            return true;
        }
    }
    else
    {
        memcpy(ringBuf+rheadPos, sdata, sdataCount);
        rheadPos += sdataCount;
    }
    return true;
}

bool RingBuff::popData(char *rData)
{
    unsigned int i = 0;
    if(rheadPos == rtailPos)
    {
        return false;
    }
    else if(rheadPos > rtailPos)
    {
        for(i=rtailPos; i < rheadPos; ++i)
        {
            if(ringBuf[i] == '\n')
            {
                memcpy(rData, ringBuf+rtailPos, i-rtailPos+1);
                rData[i-rtailPos+1] = '\0';
                rtailPos = i+1;

                return true;
            }
        }
        return false;
    }
    else
    {
        for(i=rtailPos; i < BUFF_SIZE; ++i)
        {
            if(ringBuf[i] == '\n')
            {
                memcpy(rData, ringBuf+rtailPos, i-rtailPos+1);
                rData[i-rtailPos+1] = '\0';
        //rtailPos = (i < (BUFF_SIZE-1))?(i+1):(0); //是否越界 rtailPos = i+1; rtailPos &= (BUFF_SIZE-1);
                if(i == (BUFF_SIZE -1))
                    rtailPos = 0;
                else
                    rtailPos = i+1;

                return true;
            }
        }
        for(i=0; i < rheadPos; ++i)
        {
            if(ringBuf[i] == '\n')
            {
                unsigned int temp;
                temp = BUFF_SIZE-rtailPos;
                memcpy(rData, ringBuf+rtailPos, temp);

                rtailPos = i+1;
                memcpy(rData+temp, ringBuf, rtailPos);
                temp += rtailPos;
                rData[temp] = '\0';

                return true;
            }
        }
     }
    return false;
}

bool RingBuff::isEmpty()
{
    return (rheadPos == rtailPos)?(true):(false);
}

bool RingBuff::isFull()
{
    return (rtailPos == (rheadPos+1))?(true):(false);
}

void RingBuff::setEmpty()
{
    rheadPos = 0;
    rtailPos = 0;
}

int RingBuff::getUseSize()
{
    if(rheadPos == rtailPos)
        return (BUFF_SIZE-1);
    else if(rheadPos > rtailPos)
        return (BUFF_SIZE-rheadPos+rtailPos-1);
    else
        return (rtailPos-rheadPos-1);
}

unsigned int RingBuff::getBuffSize()
{
    return BUFF_SIZE;
}








