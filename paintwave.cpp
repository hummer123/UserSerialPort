#include "paintwave.h"
#include <QtWidgets>
#include <QDebug>

#define YORGOALMAXVALUE   65536
#define YORGOALMINVALUE   10

PaintWave::PaintWave(QWidget *parent) : QWidget(parent)
{
    qDebug() << "PaintWave Init";
    isShowWave = false;
    isShowGrad = true;
    isCheckWave = false;

    initGUI();

    connect(stopShowBtn, &QPushButton::clicked, this, &PaintWave::switchShowWave);
    connect(closeWaveBtn, &QPushButton::clicked, this, &PaintWave::sendCloseWave);
    connect(gradNumSpx, SIGNAL(valueChanged(int)), paintLbl, SLOT(gradNumChange(int)));
    connect(gradInvSpx, SIGNAL(valueChanged(int)), paintLbl, SLOT(gradInvChange(int)));
    connect(yMaxValSpx, SIGNAL(valueChanged(int)), paintLbl, SLOT(yMaxValueChange(int)));
    connect(goalLineSpx, SIGNAL(valueChanged(int)), paintLbl, SLOT(goalValueChange(int)));
    connect(showGradBtn, &QPushButton::clicked, this, &PaintWave::switchShowGrad);
    connect(this, &PaintWave::sendShowGradSig, paintLbl, &PaintLabel::toShowGrad);
    connect(this, &PaintWave::sendCheckWaveSig, paintLbl, &PaintLabel::checkWaveChange);
    connect(checkWaveBtn, &QPushButton::clicked, this, &PaintWave::switchChechWave);
    connect(clearWaveBtn, &QPushButton::clicked, paintLbl, &PaintLabel::clearWaveData);

    setWindowTitle(tr("Wave Show"));
    this->setWindowIcon(QIcon(":/images/paintWave"));
}

PaintWave::~PaintWave()
{
    delete paintLbl;
}

void PaintWave::closeEvent(QCloseEvent *event)
{
    //Q_UNUSED(event);

    isShowWave = false;
    emit closeWaveSignal();
    qDebug() << "destroy paintWave";

    QWidget::closeEvent(event);
}

void PaintWave::initGUI()
{
    paintLbl = new PaintLabel(this);
    opGB = new QGroupBox(tr("Operator"));
    spGB = new QGroupBox(tr("SamplingBytes"));
    showGradBtn = new QPushButton(tr("Hide Grad"));
    gradNumLabel = new QLabel(tr("Grad Number:"));
    pointInvLabel = new QLabel(tr("Data Interval:"));
    yLabel = new QLabel(tr("Y/(10*pixel)"));
    xLabel = new QLabel(tr("X/(10*pixel)"));
    gradNumSpx = new QSpinBox;
    gradNumSpx->setRange(1,100);
    gradNumSpx->setValue(20);
    gradInvSpx = new QSpinBox;
    gradInvSpx->setRange(1,50);
    gradInvSpx->setValue(5);
    yMaxLabel = new QLabel(tr("Y MaxValue: "));
    goalLabel = new QLabel(tr("Goal Value: "));
    yMaxValSpx = new QSpinBox;
    yMaxValSpx->setRange(YORGOALMINVALUE, YORGOALMAXVALUE);
    yMaxValSpx->setValue(256);
    goalLineSpx = new QSpinBox;
    goalLineSpx->setRange(YORGOALMINVALUE, YORGOALMAXVALUE);
    goalLineSpx->setValue(120);

    QHBoxLayout *ylabelLayout = new QHBoxLayout;
    ylabelLayout->addWidget(yLabel);
    ylabelLayout->addStretch();
    QVBoxLayout *paintLayout = new QVBoxLayout;
    paintLayout->addLayout(ylabelLayout);
    paintLayout->addWidget(paintLbl);

    QHBoxLayout *numLayout = new QHBoxLayout;
    numLayout->addWidget(gradNumLabel);
    numLayout->addWidget(gradNumSpx);
    QHBoxLayout *invLayout = new QHBoxLayout;
    invLayout->addWidget(pointInvLabel);
    invLayout->addWidget(gradInvSpx);
    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(showGradBtn);
    opLayout->addLayout(numLayout);
    opLayout->addLayout(invLayout);
    opGB->setLayout(opLayout);

    QHBoxLayout *yMaxLayout = new QHBoxLayout;
    yMaxLayout->addWidget(yMaxLabel);
    yMaxLayout->addWidget(yMaxValSpx);
    QHBoxLayout *goalLayout = new QHBoxLayout;
    goalLayout->addWidget(goalLabel);
    goalLayout->addWidget(goalLineSpx);
    QVBoxLayout *samplingLayout = new QVBoxLayout;
    samplingLayout->addLayout(yMaxLayout);
    samplingLayout->addLayout(goalLayout);
    spGB->setLayout(samplingLayout);

    QHBoxLayout *xlabelLayout = new QHBoxLayout;
    xlabelLayout->addWidget(xLabel);
    xlabelLayout->addStretch();

    checkWaveBtn = new QPushButton(tr("Check History Wave"));
    checkWaveBtn->setFixedHeight(30);
    stopShowBtn = new QPushButton(tr("Start Show Wave"));
    stopShowBtn->setFixedHeight(30);
    clearWaveBtn = new QPushButton(tr("Clear Wave Data"));
    clearWaveBtn->setFixedHeight(30);
    closeWaveBtn = new QPushButton(tr("Close Wave"));
    closeWaveBtn->setIcon(QIcon(":/images/closeWave"));
    closeWaveBtn->setFixedHeight(50);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(opGB);
    rightLayout->addWidget(spGB);
    rightLayout->addStretch(3);
    rightLayout->addWidget(checkWaveBtn, Qt::AlignCenter);
    rightLayout->addWidget(stopShowBtn, Qt::AlignCenter);
    rightLayout->addWidget(clearWaveBtn, Qt::AlignCenter);
    rightLayout->addWidget(closeWaveBtn, Qt::AlignCenter);
    rightLayout->addStretch(1);
    rightLayout->addLayout(xlabelLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(paintLayout);
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);
}

void PaintWave::handleWaveItem(const char *sData)
{
    if(isShowWave)
    {
        int num;
        float sVal;
        char op;
        op = sData[0];
        if(op == '#')
        {
            num = sscanf(&sData[1], "%f", &sVal);
            if(num != 1)
                return;
            paintLbl->addItemToList(sVal);
        }
    }
}

void PaintWave::switchShowWave()
{
    if(isShowWave)
    {
        stopShowBtn->setText(tr("Start Show Wave"));
        isShowWave = false;
    }
    else
    {
        stopShowBtn->setText(tr("Stop Show Wave"));
        isShowWave = true;
    }
}

void PaintWave::sendCloseWave()
{
    isShowWave = false;
    emit closeWaveSignal();
}

void PaintWave::switchShowGrad()
{
    if(isShowGrad)
    {
        showGradBtn->setText(tr("Show Grad"));
        emit sendShowGradSig();
        isShowGrad = false;
    }
    else
    {
        showGradBtn->setText(tr("Hide Grad"));
        emit sendShowGradSig();
        isShowGrad = true;
    }
}

void PaintWave::switchChechWave()
{
    if(isCheckWave)
    {
        checkWaveBtn->setText(tr("Check History Wave"));
        isCheckWave = false;
        emit sendCheckWaveSig();
    }
    else
    {
        checkWaveBtn->setText(tr("RealTimer Show Wave"));
        isCheckWave = true;
        emit sendCheckWaveSig();
    }
}


/*
 * Painter QLabel
 *
*/
PaintLabel::PaintLabel(QWidget *parent) : QLabel(parent)
{
    qDebug() << "PaintLabe Init";
    rect=QRect(0, 0, WAVE_WIDTH, WAVE_HEIGHT);
    initPoint.setX(20);
    initPoint.setY(10);
    halfHight = WAVE_HEIGHT/2;
    yMaxValue = 256;
    goalValue = 120;
    pointInterval = 5;
    isShowGrid = true;
    isCheckWave = false;
    gridnum = 20;
    leftPos = 0;
    rightPos = 0;
    hstyDst = 0;
    painter = NULL;

    this->setFrameShape(QFrame::Box);
    this->setStyleSheet("border:2px solid rgb(255,0,0)");
    this->setFixedSize(WAVE_WIDTH+30, WAVE_HEIGHT+30);
    //resize(WAVE_WIDTH + 210, WAVE_HEIGHT + 100);
}

PaintLabel::~PaintLabel()
{
    qDebug() << "painter Act";
    if(painter != NULL)
        delete painter;
    qDebug() << "painter end";
}

void PaintLabel::paintEvent(QPaintEvent *event)
{
    int tempVal;
    painter = new QPainter(this);
    painter->translate(initPoint.x(), initPoint.y());
    painter->fillRect(rect, QBrush(QColor(Qt::black),Qt::SolidPattern));

    setXYCord();
    if(isShowGrid)
    {
        showGrid();
    }
    //y轴中线
    painter->setPen(QPen(Qt::magenta));
    painter->drawLine(rect.x(), halfHight, WAVE_WIDTH, halfHight);

    //目标曲线
    painter->setPen(QPen(Qt::red));
    tempVal = halfHight*(yMaxValue - goalValue)/yMaxValue;
    painter->drawLine(rect.x(), tempVal, WAVE_WIDTH, tempVal);

    if(!isCheckWave)
        realTimerWave();
    else
        historyTimeWave();
    //RealTimeWave();
    painter->end();

    QLabel::paintEvent(event);
}

void PaintLabel::mousePressEvent(QMouseEvent *event)
{
    startPos = event->x();
}

void PaintLabel::mouseMoveEvent(QMouseEvent *event)
{
    moveDst = event->x() - startPos;
    update();
}

void PaintLabel::mouseReleaseEvent(QMouseEvent *event)
{
     hstyDst += (event->x() - startPos);
}

void PaintLabel::addItemToList(float vdata)
{
    int moveDis;

    // x move pointInterval
    for(int i = 0; i < historyValList.count(); ++i)
    {
        moveDis = historyValList[i].x() - pointInterval;
        historyValList[i].setX(moveDis);
    }
    newVal.setX(WAVE_WIDTH);
    newVal.setY(vdata);

    if(vdata > yMaxValue)
        vdata = yMaxValue;
    else if(vdata < -yMaxValue)
        vdata = -yMaxValue;

    historyValList.append(newVal);

    if(historyValList.count() > 4096)
        historyValList.removeFirst();

    if(!isCheckWave)    //如果是查看数据曲线则在添加数据时不更新曲线
        update();
    //repaint();
}

void PaintLabel::toShowGrad()
{
    if(isShowGrid == true)
        isShowGrid = false;
    else
        isShowGrid = true;

    update();
}

void PaintLabel::gradNumChange(int sNum)
{
    gridnum = sNum;

    update();
}

void PaintLabel::gradInvChange(int sNum)
{
    pointInterval = sNum;

    update();
}

void PaintLabel::yMaxValueChange(int yVal)
{
    if(yVal < YORGOALMINVALUE)
        yMaxValue = YORGOALMINVALUE;
    else if(yVal > YORGOALMAXVALUE)
        yMaxValue = YORGOALMAXVALUE;
    else
        yMaxValue = yVal;

    update();
}

void PaintLabel::goalValueChange(int gVal)
{
    if(gVal > yMaxValue)
    {
        goalValue = (yMaxValue - 5);
    }
    else
    {
        goalValue = gVal;
    }

    update();
}

void PaintLabel::checkWaveChange()
{
    if(isCheckWave)
    {
        isCheckWave = false;
        update();
    }
    else
        isCheckWave = true;
}

void PaintLabel::clearWaveData()
{
    historyValList.clear();
    leftPos = 0;
    rightPos = 0;
    hstyDst = 0;
    update();
}

void PaintLabel::setXYCord()
{
    //x
    painter->setPen(QPen(Qt::yellow));
    for(int i = 1; i <= WAVE_WIDTH/10 ;i++)
    {
        point[0].setX(i*10);
        point[0].setY(WAVE_HEIGHT);
        point[1].setX(i*10);
        point[1].setY(WAVE_HEIGHT-3);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    painter->drawLine(QPoint(0,WAVE_HEIGHT),QPoint(WAVE_WIDTH, WAVE_HEIGHT));

    //y
    painter->setPen(QPen(Qt::red));
    for(int i=WAVE_HEIGHT/10 - 1; i>=0; i--)
    {
        point[0].setX(0);
        point[0].setY(i*10);
        point[1].setX(3);
        point[1].setY(i*10);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    painter->drawLine(QPoint(0,0),QPoint(0,WAVE_HEIGHT));

    //y值
    QFont font("Arial",6,QFont::Normal,true);
    painter->setFont(font);
    for(int i = 0; i < WAVE_HEIGHT; i += 10)
    {
        painter->drawText(rect.x()-13, rect.y()+i+3, QString::number((WAVE_HEIGHT/2 - i)/10, 10));
    }

    //x值
    painter->setFont(font);
    painter->setPen(QPen(Qt::blue));
    for(int i=10, j = 1; i <= WAVE_WIDTH; i += 10)
    {
        painter->drawText(rect.x()+i-3, WAVE_HEIGHT+13, QString::number(j,10));
        j++;
    }
}

void PaintLabel::showGrid()
{
    //x
    painter->setPen(QPen(Qt::darkGreen));

    //x
    point[0].setY(0);
    point[1].setY(WAVE_HEIGHT-3);

    for(int i=1; i <= WAVE_WIDTH/gridnum; i++)
    {
        point[0].setX(i*gridnum);
        point[1].setX(i*gridnum);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    //y
    point[0].setX(3);
    point[1].setX(WAVE_WIDTH);
    for(int i = WAVE_HEIGHT/gridnum; i > 0; i--)
    {
        point[0].setY(i*gridnum);
        point[1].setY(i*gridnum);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
}

void PaintLabel::realTimerWave()
{
    QPointF curPoint, oldPoint;
    float tempVal;
    int xP;
    if(historyValList.count() < 2)
        return;

    for(xP = historyValList.count()-1; xP > 0; --xP)
    {
        if(historyValList[xP].x() <= 0)
            break;
    }

    leftPos = xP;

    oldPoint.setX(historyValList[xP].x());
    tempVal = halfHight*(yMaxValue - historyValList[xP].y())/yMaxValue;
    oldPoint.setY(tempVal);
    //painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::yellow));
    for(int j=xP+1; j < historyValList.count(); ++j)
    {
        curPoint.setX(historyValList[j].x());
        tempVal = halfHight*(yMaxValue - historyValList[j].y())/yMaxValue;
        curPoint.setY(tempVal);
        painter->drawLine(oldPoint, curPoint);
        oldPoint = curPoint;
    }

    rightPos = historyValList.count()-1;
    hstyDst = 0;
}

void PaintLabel::historyTimeWave()
{
    QPointF curPoint, oldPoint;
    float tempVal;
    int detaDst;

    if(historyValList.isEmpty())
        return;

    detaDst = hstyDst + moveDst;

    if(moveDst >= 0)
    {
        for(int i = leftPos; i >= 0; --i)
        {
            if((historyValList[i].x() + detaDst) <= 0)
            {
                leftPos = i;
                break;
            }
        }
        oldPoint.setX(historyValList[leftPos].x()+detaDst);
        tempVal = halfHight*(yMaxValue - historyValList[leftPos].y())/yMaxValue;
        oldPoint.setY(tempVal);
        painter->setPen(QPen(Qt::yellow));
        for(int i=leftPos+1; i < historyValList.count(); ++i)
        {
            if(historyValList[i].x()+detaDst >= WAVE_WIDTH)
            {
                rightPos = i;
                break;
            }
            curPoint.setX(historyValList[i].x()+detaDst);
            tempVal = halfHight*(yMaxValue - historyValList[i].y())/yMaxValue;
            curPoint.setY(tempVal);
            painter->drawLine(oldPoint, curPoint);
            oldPoint = curPoint;
        }
    }
    else if(moveDst < 0)
    {
        for(int i = rightPos; i < historyValList.count(); ++i)
        {
            if((historyValList[i].x() + detaDst) >= WAVE_WIDTH)
            {
                rightPos = i;
                break;
            }
        }
        oldPoint.setX(historyValList[rightPos].x()+detaDst);
        tempVal = halfHight*(yMaxValue - historyValList[rightPos].y())/yMaxValue;
        oldPoint.setY(tempVal);
        painter->setPen(QPen(Qt::yellow));
        for(int i=rightPos-1; i >= 0; --i)
        {
            if((historyValList[i].x() + detaDst) <= 0)
            {
                leftPos = i;
                break;
            }

            curPoint.setX(historyValList[i].x()+detaDst);
            tempVal = halfHight*(yMaxValue - historyValList[i].y())/yMaxValue;
            curPoint.setY(tempVal);
            painter->drawLine(curPoint, oldPoint);
            oldPoint = curPoint;
        }
    }
}


#if 0
void PaintLabel::RealTimeWave()
{
    QPoint curPoint, oldPoint;
    int tempVal;
    if(historyValList.count() < 2)
        return;

    oldPoint.setX(historyValList[0].x());
    tempVal = halfHight*(yMaxValue - historyValList[0].y())/yMaxValue;
    oldPoint.setY(tempVal);
    //painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::green));
    for(int j=1; j < historyValList.count(); ++j)
    {
        curPoint.setX(historyValList[j].x());
        tempVal = halfHight*(yMaxValue - historyValList[j].y())/yMaxValue;
        curPoint.setY(tempVal);
        painter->drawLine(oldPoint, curPoint);
        oldPoint = curPoint;
    }

//    //painter->setRenderHint(QPainter::Antialiasing, true);
//    oldPoint.setY(halfHight);
//    painter->setPen(QPen(Qt::red));
//    for(int j=0; j < historyValList.count(); ++j)
//    {
//        oldPoint.setX(historyValList[j].x());
//        curPoint.setX(historyValList[j].x());

//        tempVal = halfHight*(yMaxValue - historyValList[j].y())/y;
//        curPoint.setY(tempVal);
//        painter->drawLine(oldPoint, curPoint);
//        //oldPoint = curPoint;
//    }
}
#endif

//void PaintLabel::testAddItem()
//{
//    static unsigned int xItem = 0;
//    unsigned int temp;

//    ++xItem;
//    temp = (int)200*sin(xItem*PI/10);

//    addItemToList(temp);
//    qDebug() << "paintlabel ThreadID:" << QThread::currentThreadId();
//}


