#include "mainwindow.h"
#include <QtWidgets>
#include <QScrollBar>
#include <QtCore/QDebug>


const QString conStr("0123456789ABCDEF");


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    initUI(mainWidget);
    filePortParameters();

    paintWave = new PaintWave;
    paintWave->hide();
    qDebug() << "Creator paintWave";

    myCom = NULL;
    serialThread = NULL;

    initConnect();
    sendEdit->installEventFilter(this);
    saveFilePathLineEdit->installEventFilter(this);

    setWindowTitle(tr("SerialPort"));
    this->setWindowIcon(QIcon(":/images/serialMain"));
}

MainWindow::~MainWindow()
{
    if(myCom != NULL)
    {
        if(myCom->isOpen())
            myCom->close();
        delete myCom;
        qDebug() << "mainwindow.cpp---Delete myCom";
    }
    if(serialThread != NULL)
    {
        //if(serialThread->isRunning())
        serialThread->stopThread();
        delete serialThread;
        serialThread = NULL;
        qDebug() << "mainwindow.cpp---Delete serialThread";
    }
    qDebug() << "Ready to delete paintWave";
    delete paintWave;
    qDebug() << "Delete paintWave";
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == sendEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return))
            {
                serialWriteDate();
                //qDebug() << "key_enter";
                return true;
            }
        }
    }
    else if(obj == saveFilePathLineEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyAnyEvent = static_cast<QKeyEvent *>(event);
            if((keyAnyEvent->key() == Qt::Key_Left) || (keyAnyEvent->key() == Qt::Key_Right))
            {
                //qDebug() << "Key_Left";
                return QObject::eventFilter(obj,event);
            }
            else
                return true;
        }
    }

    return QObject::eventFilter(obj,event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    paintWave->hide();
    QMainWindow::closeEvent(event);
}

void MainWindow::initUI(QWidget *ctlWidget)
{
    //Receive Data Area
    receiveDataGBx = new QGroupBox(tr("Receive Data"));
    receiveEdit = new QTextEdit;
    receiveEdit->setReadOnly(true);
    QVBoxLayout *editLayout = new QVBoxLayout;
    //editLayout->setMargin(2);
    editLayout->addWidget(receiveEdit);
    receiveDataGBx->setLayout(editLayout);
    //SerialPort Configure Area
    portSelectGBx = new QGroupBox(tr("Serial Port Info"));
    serialPortCBx = new QComboBox;
    baudRateCBx = new QComboBox;
    stopBitsCBX = new QComboBox;
    dataBitsCBx = new QComboBox;
    parityCBx = new QComboBox;
    flowControlCBx = new QComboBox;
    baudRatelabel = new QLabel(tr("BaudRate: "));
    stopBitslabel = new QLabel(tr("Stop bits: "));
    dataBitslabel = new QLabel(tr("Data bits: "));
    paritylabel = new QLabel(tr("Parity: "));
    flowControllabel = new QLabel(tr("Flow Control:"));
    openPortBtn = new QPushButton(tr("Open Serial Port"));
    clearReceiveDataBtn = new QPushButton(tr("Clear Receive Data"));
    showWaveBtn = new QPushButton(tr("Show Waveform"));
    QGridLayout *topRightLayout = new QGridLayout;
    //topRightLayout->setMargin(2);
    topRightLayout->addWidget(serialPortCBx,0, 0, 1, 2);
    topRightLayout->addWidget(baudRatelabel, 1, 0);
    topRightLayout->addWidget(baudRateCBx, 1, 1);
    topRightLayout->addWidget(dataBitslabel, 2, 0);
    topRightLayout->addWidget(dataBitsCBx, 2, 1);
    topRightLayout->addWidget(stopBitslabel, 3, 0);
    topRightLayout->addWidget(stopBitsCBX, 3, 1);
    topRightLayout->addWidget(paritylabel, 4, 0);
    topRightLayout->addWidget(parityCBx, 4, 1);
    topRightLayout->addWidget(flowControllabel, 5, 0);
    topRightLayout->addWidget(flowControlCBx, 5, 1);
    topRightLayout->addWidget(openPortBtn, 6, 0, 1, 2);
    topRightLayout->addWidget(clearReceiveDataBtn, 7, 0, 1, 2);
    topRightLayout->addWidget(showWaveBtn, 8, 0, 1, 2);
    portSelectGBx->setLayout(topRightLayout);
    //#combine< Receive Data Area -and- SerialPort configure Area >
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setMargin(1);
    topLayout->addWidget(receiveDataGBx);
    topLayout->addWidget(portSelectGBx);


    //Send Data Area
    sendDataGBx = new QGroupBox(tr("Send Data"));
    sendPortBtn = new QPushButton(tr("Send Data"));
    clearSendDataBtn = new QPushButton(tr("Clear Send Data"));
    sendEdit = new QTextEdit;
    sendLine = new QLineEdit;
    //sendLine->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    sendLineBtn = new QPushButton(tr(" Send Line Data"));
    QGridLayout *sendDataTopLayout = new QGridLayout;
    sendDataTopLayout->addWidget(sendEdit, 0, 0, 2, 1);
    sendDataTopLayout->addWidget(sendPortBtn, 0, 1);
    sendDataTopLayout->addWidget(clearSendDataBtn, 1, 1);
    QHBoxLayout *sendLineLayout = new QHBoxLayout;
    sendLineLayout->addWidget(sendLine);
    sendLineLayout->addWidget(sendLineBtn);
    //Save File Area
    saveFileBtn = new QPushButton(tr("Save data to file"));
    importFileBtn = new QPushButton(tr("Send File"));
    importFileBtn->setEnabled(false);
    openFileBtn = new QPushButton(tr("Open File"));
    saveFilePathLineEdit = new QLineEdit;
    saveFilePathLineEdit->setStyleSheet("QLineEdit{border:1px solid #e1e0e0}");
    //saveFilePathLineEdit->setText(tr(" files "));
    QHBoxLayout *sendDataBtnLayout = new QHBoxLayout;
    sendDataBtnLayout->addWidget(openFileBtn);
    sendDataBtnLayout->addWidget(importFileBtn);
    sendDataBtnLayout->addWidget(saveFileBtn);
    //#combine< Send Data Area -and- Save File Area >
    QVBoxLayout *sendDataLayout = new QVBoxLayout;
    sendDataLayout->setMargin(2);
    sendDataLayout->addLayout(sendDataTopLayout);
    sendDataLayout->addLayout(sendLineLayout);
    sendDataLayout->addWidget(saveFilePathLineEdit);
    sendDataLayout->addLayout(sendDataBtnLayout);
    sendDataGBx->setLayout(sendDataLayout);


    //Other Information Area
    otherInfoGBx = new QGroupBox(tr("Other Info"));
    hexReceiveCKx = new QCheckBox(tr("Hex Receive"));
    hexSendCKx = new QCheckBox(tr("Hex Send"));
    sendNewLineCKx = new QCheckBox(tr("Send a newline"));
    sendNewLineCKx->setChecked(true);
    clearStatusCntBtn = new QPushButton(tr("Clear Status Count"));
    clearStatusCntBtn->setFixedHeight(30);
    autoSendCKx = new QCheckBox(tr("Auto Send Cycle(ms):"));
    autoSendTimeSPx = new QSpinBox;
    autoSendTimeSPx->setRange(MIN_AUTO_TIME, MAX_AUTO_TIME);
    autoSendTimeSPx->setValue(1000);
    QHBoxLayout *sendChekLayout = new QHBoxLayout;
    sendChekLayout->addWidget(autoSendCKx);
    sendChekLayout->addWidget(autoSendTimeSPx);
    QVBoxLayout *buttonRightLayout = new QVBoxLayout;
    buttonRightLayout->addWidget(hexSendCKx);
    buttonRightLayout->addWidget(hexReceiveCKx);
    buttonRightLayout->setMargin(2);
    buttonRightLayout->addWidget(sendNewLineCKx);
    buttonRightLayout->addLayout(sendChekLayout);
    buttonRightLayout->addWidget(clearStatusCntBtn);
    otherInfoGBx->setLayout(buttonRightLayout);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(2);
    buttonLayout->addWidget(sendDataGBx);
    buttonLayout->addWidget(otherInfoGBx);


    //#combine all Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(2);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);

    portStatuslabel = new QLabel(tr("  Serial Port Close"));
    sendCountlabel  = new QLabel(tr("Send: 0 Byte"));
    ReceiveCountlabel = new QLabel(tr("Receive: 0 Byte"));
    currentTimelabel  = new QLabel(tr("Time: 0:0"));
    QTime timeNow = QTime::currentTime();
    currentTimelabel->setText(QString("Time: %1 ").arg(timeNow.toString()));
    statusBar()->addWidget(portStatuslabel, 1);
    statusBar()->addWidget(sendCountlabel,1);
    statusBar()->addWidget(ReceiveCountlabel, 1);
    statusBar()->addWidget(currentTimelabel, 1);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    ctlWidget->setLayout(mainLayout);

    QPalette receiveP = receiveEdit->palette();
    receiveP.setColor(QPalette::Base, Qt::black);
    receiveP.setColor(QPalette::Text, Qt::green);
    receiveEdit->setPalette(receiveP);

    QPalette sendP = sendEdit->palette();
    sendP.setColor(QPalette::Base, Qt::black);
    sendP.setColor(QPalette::Text, Qt::green);
    sendEdit->setPalette(sendP);

    QPalette LineP = sendLine->palette();
    LineP.setColor(QPalette::Base, Qt::black);
    LineP.setColor(QPalette::Text, Qt::green);
    sendLine->setPalette(LineP);
}

void MainWindow::initConnect()
{
    autoSendTimer = new QTimer(this);
    autoSendTimer->setInterval(1000);
    connect(autoSendTimer, &QTimer::timeout, this, &MainWindow::serialWriteDate);
    connect(autoSendTimeSPx, SIGNAL(valueChanged(int)), this, SLOT(changeAutoSendTime(int)));
    connect(autoSendCKx, &QCheckBox::clicked, this, &MainWindow::isAutoSend);

    timerShow = new QTimer(this);
    timerShow->start(1000);
    connect(timerShow, &QTimer::timeout, this, &MainWindow::setTime);

    connect(openPortBtn, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(sendPortBtn, &QPushButton::clicked, this, &MainWindow::SendSerialData);
    connect(showWaveBtn, &QPushButton::clicked, this, &MainWindow::openWave);
    connect(showWaveBtn, &QPushButton::clicked, paintWave, &PaintWave::switchShowWave);
    connect(clearReceiveDataBtn, &QPushButton::clicked, this, &MainWindow::clearReceiveArea);
    connect(clearSendDataBtn, &QPushButton::clicked, this, &MainWindow::clearSendArea);
    connect(clearStatusCntBtn, &QPushButton::clicked, this, &MainWindow::clearStatusArea);
    connect(saveFileBtn, &QPushButton::clicked, this, &MainWindow::saveToFile);
    connect(importFileBtn, &QPushButton::clicked, this, &MainWindow::sendFile);
    connect(openFileBtn, &QPushButton::clicked, this, &MainWindow::openFileAPath);
    connect(paintWave, &PaintWave::closeWaveSignal, this, &MainWindow::openWave); //wave close format
    connect(sendLineBtn, &QPushButton::clicked, this, &MainWindow::serialWriteLineData);
}

void MainWindow::filePortParameters()
{
    //set Port Item
#ifdef Q_OS_LINUX
   serialPortCBx->addItem("ttyUSB0");
   serialPortCBx->addItem("ttyUSB1");
   serialPortCBx->addItem("ttyUSB2");
   serialPortCBx->addItem("ttyUSB3");
   serialPortCBx->addItem("ttyS0");
   serialPortCBx->addItem("ttyS1");
   serialPortCBx->addItem("ttyS2");
   serialPortCBx->addItem("ttyS3");
   serialPortCBx->addItem("ttyS4");
   serialPortCBx->addItem("ttyS5");
   serialPortCBx->addItem("ttyS6");
#elif defined (Q_OS_WIN)
   serialPortCBx->addItem("COM0");
   serialPortCBx->addItem("COM1");
   serialPortCBx->addItem("COM2");
   serialPortCBx->addItem("COM3");
   serialPortCBx->addItem("COM4");
   serialPortCBx->addItem("COM5");
   serialPortCBx->addItem("COM6");
   serialPortCBx->addItem("COM7");
   serialPortCBx->addItem("COM8");
   serialPortCBx->addItem("COM9");
   serialPortCBx->addItem("COM10");
   serialPortCBx->addItem("COM11");
   serialPortCBx->addItem("COM12");
   serialPortCBx->addItem("COM13");
   serialPortCBx->addItem("COM14");
   serialPortCBx->addItem("COM15");
#endif
   serialPortCBx->setCurrentIndex(0);

   baudRateCBx->addItem("1200");
   baudRateCBx->addItem("2400");
   baudRateCBx->addItem("4800");
   baudRateCBx->addItem("9600");
   baudRateCBx->addItem("14400");
   baudRateCBx->addItem("19200");
   baudRateCBx->addItem("38400");
   baudRateCBx->addItem("56000");
   baudRateCBx->addItem("115200");
   baudRateCBx->addItem("128000");
   baudRateCBx->addItem("256000");
   baudRateCBx->setCurrentIndex(8);

   stopBitsCBX->addItem("1");
#ifdef Q_OS_WIN
   stopBitsCBX->addItem("1.5");
#endif
   stopBitsCBX->addItem("2");
   stopBitsCBX->setCurrentIndex(0);

   dataBitsCBx->addItem("5");
   dataBitsCBx->addItem("6");
   dataBitsCBx->addItem("7");
   dataBitsCBx->addItem("8");
   dataBitsCBx->setCurrentIndex(3);

   parityCBx->addItem("None");
   parityCBx->addItem("Even");
   parityCBx->addItem("Odd");
   parityCBx->addItem("Space");
#ifdef Q_OS_WIN
   parityCBx->addItem("Mark");
#endif
   parityCBx->setCurrentIndex(0);

   flowControlCBx->addItem("None");
   flowControlCBx->addItem("RTS/CTS");
   flowControlCBx->addItem("XON/XOFF");
   flowControlCBx->setCurrentIndex(0);
}

void MainWindow::setTime()
{
    QTime curTime = QTime::currentTime();
    currentTimelabel->setText(QString("Time: %1").arg(curTime.toString()));
}

void MainWindow::openSerialPort()
{
    if(isOpenSerial == false)
    {
        QString portName = serialPortCBx->currentText();
#ifdef Q_OS_LINUX
        myCom = new QextSerialPort("/dev/" + portName);
#elif  defined (Q_OS_WIN)
        myCom = new QextSerialPort(portName);
#endif
        qDebug() << "mainwindow.cpp---Creator myCom";
        //####connect(myCom, &QextSerialPort::readyRead, this, &MainWindow::serialReadDate);

        myCom->setBaudRate((BaudRateType)baudRateCBx->currentText().toInt());
        myCom->setDataBits((DataBitsType)dataBitsCBx->currentText().toInt());

        switch(stopBitsCBX->currentIndex())
        {
        case 0: myCom->setStopBits(STOP_1); break;
        case 1:
#ifdef Q_OS_WIN
                myCom->setStopBits(STOP_1_5);
#endif
                break;
        case 2: myCom->setStopBits(STOP_2); break;
        default: myCom->setStopBits(STOP_1);
                qDebug() << "set to default : STOP_1"; break;
        }

        switch(parityCBx->currentIndex())
        {
        case 0: myCom->setParity(PAR_NONE); break;
        case 1: myCom->setParity(PAR_EVEN); break;
        case 2: myCom->setParity(PAR_ODD);  break;
        case 3: myCom->setParity(PAR_SPACE);break;
#ifdef Q_OS_WIN
        case 4: myCom->setParity(PAR_MARK); break;
#endif
        default: myCom->setParity(PAR_NONE);
            qDebug() << "set to default : PAR_NONE"; break;
        }

        switch(flowControlCBx->currentIndex())
        {
        case 0: myCom->setFlowControl(FLOW_OFF); break;
        case 1: myCom->setFlowControl(FLOW_HARDWARE); break;
        case 2: myCom->setFlowControl(FLOW_XONXOFF); break;
        default: myCom->setFlowControl(FLOW_OFF);
            qDebug() << "set to default : STOP_1"; break;
        }

        // set SerialPort TimeOut
        myCom->setTimeout(TIME_OUT);

        if(myCom->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this, tr("OPen Serial Successful"), tr("OPen Serial %1 Successful").arg(portName), QMessageBox::Ok);
            portStatuslabel->setText(tr("  %1 is Open").arg(portName));

            //Thread receive and del Data
            serialThread = new ReceiveThread(myCom, this);
            connect(serialThread, SIGNAL(serialFinished(const char*)), this, SLOT(serialReadDate(const char*)), Qt::QueuedConnection);
            connect(serialThread, SIGNAL(serialFinished(const char*)), paintWave, SLOT(handleWaveItem(const char*)), Qt::QueuedConnection);
            serialThread->start(QThread::NormalPriority);
            qDebug() << "mainwindow.cpp---Creator serialThread";

            isOpenSerial = true;
            openPortBtn->setText(tr("Close Serial Port"));
            setOpenObjectStatus(false);
        }
        else
        {
            QMessageBox::information(this, tr("Open Serial failed"), tr("Con't Open ") + portName, QMessageBox::Ok);

            //delete myCom
            delete myCom;
            myCom = NULL;
            qDebug() << "mainwindow.cpp---Delete myCom";
            return;
        }
    }
    else
    {
        //end Thread
        qDebug() << "serialThread Action end";
        serialThread->stopThread();
        delete serialThread;
        serialThread = NULL;
        qDebug() << "mainwindow.cpp---Delete serialThread";

        if(myCom->isOpen())
            myCom->close();

        //delete myCom
        delete myCom;
        myCom = NULL;
        qDebug() << "mainwindow.cpp---Delete myCom";

        autoSendTimer->stop();  //stop AutoSend
        qDebug() << "Stop autoSend";
        isOpenSerial = false;

        portStatuslabel->setText(tr("  Serial Port is Stop"));
        openPortBtn->setText(tr("Open Serial Port"));
        setOpenObjectStatus(true);
    }
}

void MainWindow::setOpenObjectStatus(bool sta)
{
    serialPortCBx->setEnabled(sta);
    baudRateCBx->setEnabled(sta);
    dataBitsCBx->setEnabled(sta);
    stopBitsCBX->setEnabled(sta);
    parityCBx->setEnabled(sta);
    flowControlCBx->setEnabled(sta);

    importFileBtn->setEnabled(!sta);
}

/*
 *    测试绘图，绘制正玄波
 *     ++s_sendCnt;
 *   if(s_sendCnt >= 2000)
 *       s_sendCnt = 0;
 *   temp = (int)200*sin(s_sendCnt*PI/10);
 *
 */
void MainWindow::serialWriteDate()
{
    QString sendBuff = sendEdit->toPlainText();
    QByteArray sendHexBuff;

    if(sendBuff.isEmpty())
    {
        sendEdit->setFocus();
        return;
    }
    if(!myCom->isOpen())
        return;

    if(hexSendCKx->isChecked())
    {
        QStringList listBuff;
        QString listStr;
        char byteData;
        bool isOk;

        listBuff = sendBuff.split(" ");
        for(int i = 0; i < listBuff.count(); ++i)
        {
            listStr = listBuff.at(i);
            if((listStr == " ") || (listStr.isEmpty()))
                continue;

            byteData = (char)listStr.toInt(&isOk, 16);
            if(!isOk)
            {
                QMessageBox::information(this, tr("Send Data Input Error"),
                                         tr("Send Data Format Error"), QMessageBox::Ok);
                if(autoSendTimer->isActive())
                    autoSendTimer->stop();
                return;
            }
            sendHexBuff.append(byteData);
        }
    }
    else
    {
        //sendBuff.append(QString::number(temp)); //测试绘图时使用
        sendHexBuff = sendBuff.toLocal8Bit();
    }

    if(sendNewLineCKx->isChecked())
    {
#ifdef Q_OS_WIN
        sendHexBuff.append("\r\n");
#elif defined (Q_OS_LINUX)
        sendHexBuff.append('\n');
#endif
    }

    myCom->write(sendHexBuff);
    //qDebug() << "Send " << s_sendCnt << sendHexBuff.size() << "Bytes ->" << sendHexBuff;
    sendCount += (unsigned int)sendHexBuff.size();
    sendCountlabel->setText(tr("Send: %1 Byte ").arg(sendCount));
}

void MainWindow::serialReadDate(const char *sData)
{
    QString hexStr;

    if(hexReceiveCKx->isChecked())
    {
        int sDataLen = strlen(sData);
        int bt;
        for(int i=0; i < sDataLen-1; ++i)
        {
            bt = 0x0F&(sData[i]>>4);
            hexStr.append(conStr.at(bt));
            bt = 0x0F&sData[i];
            hexStr.append(conStr.at(bt));
            hexStr.append(" ");
        }
        bt = 0x0F&(sData[sDataLen-1]>>4);
        hexStr.append(conStr.at(bt));
        bt = 0x0F&sData[sDataLen-1];
        hexStr.append(conStr.at(bt));

        //receiveEdit->setText(receiveEdit->document()->toPlainText() + hexStr);
        receiveEdit->insertPlainText(hexStr);
    }
    else
    {
        hexStr.append(sData);
        //receiveEdit->setText(receiveEdit->document()->toPlainText() + hexStr);
        receiveEdit->insertPlainText(hexStr);
    }

    if(!writeToFileName.isEmpty())
    {
        QFile savefile(writeToFileName);

        if(!savefile.open(QFile::WriteOnly | QIODevice::Append | QIODevice::Text)) //追加到文本末尾
        {
            QMessageBox::information(this, tr("Save File Failed"), tr("Open file %1 failed, cannot save file\n%2")
                                                 .arg(writeToFileName).arg(savefile.errorString()), QMessageBox::Ok);
            return;
        }
        QTextStream out(&savefile);
        out<< QTime::currentTime().toString("HH:mm:ss") << "  " << hexStr;
        savefile.close();
    }

    //qDebug() << "Receive" << hexStr.size() << "->" << hexStr;

    //设置竖直滚条在最下方，显示TextEdit最新数据
    QTextCursor cursor = receiveEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    receiveEdit->setTextCursor(cursor);

    receiveCount += (unsigned int)hexStr.size();
    ReceiveCountlabel->setText(tr("Receive: %1 Byte ").arg(receiveCount));
}

void MainWindow::changeAutoSendTime(int val)
{
    if(val < MIN_AUTO_TIME)
        val = MIN_AUTO_TIME;
    else if(val > MAX_AUTO_TIME)
        val = MAX_AUTO_TIME;

    autoSendTimer->setInterval(val);
}

void MainWindow::isAutoSend()
{
    if(!autoSendCKx->isChecked())
    {
        autoSendTimer->stop();
    }
}

void MainWindow::clearReceiveArea()
{
    receiveEdit->clear();
}

void MainWindow::clearSendArea()
{
    sendEdit->clear();
}

void MainWindow::clearStatusArea()
{
    sendCount = 0;
    receiveCount = 0;
    sendCountlabel->setText(tr("Send: %1 Byte").arg(sendCount));
    ReceiveCountlabel->setText(tr("Receive: %1 Byte").arg(receiveCount));
}

void MainWindow::saveToFile()
{
    if(isSaveFile == false)
    {
        QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save As..."), ".", tr("Save As Files(*.txt);;Save All Files(*.)"));
        if(saveFilePath.isEmpty())
            return;
        QFile savefile(saveFilePath);
        if(!savefile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this, tr("Save File Failed"), tr("Open file %1 failed, cannot save file\n%2")
                                     .arg(saveFilePath).arg(savefile.errorString()), QMessageBox::Ok);
            return;
        }
        writeToFileName = saveFilePath;
        saveFileBtn->setText(tr("Stop Save Data"));
        //saveFileBtn->setStyleSheet("background-color: rgb(170,0,255);");
        saveFilePathLineEdit->setText("SaveAs: " + QFileInfo(savefile).canonicalFilePath());
        savefile.close();
        isSaveFile = true;
    }
    else
    {
        writeToFileName.clear();
        saveFileBtn->setText(tr("Save data to file"));
        isSaveFile = false;
    }
}

void MainWindow::openFileAPath()
{
    QString importFilePath = QFileDialog::getOpenFileName(this, tr("Open File..."), ".", tr("txt File (*.txt);;log File (*.log)"));
    if(importFilePath.isEmpty())
        return;
    QFile importfile(importFilePath);
    if(!importfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Import File Failed"), tr("Open file %1 failed, cannot save file\n%2")
                             .arg(importFilePath).arg(importfile.errorString()), QMessageBox::Ok);
        importfile.close();
        return;
    }
    if(importfile.size() > MAX_FILE_SIZE)
    {
        QMessageBox::critical(this, tr("File too large"), tr("File size is %1, Over Max file size %2")
                              .arg(importfile.size()).arg(MAX_FILE_SIZE), QMessageBox::Ok);
        importfile.close();
        return;
    }
    else if(importfile.size() > TIP_FILE_SIZE)
    {
        if(QMessageBox::question(this, tr("File too large"), tr("File size too large, Confirm whether to open"),
                              QMessageBox::Ok | QMessageBox::No) == QMessageBox::No)
        importfile.close();
        return;
    }

    importFromFileName = importFilePath;
    if(importFromFileName == writeToFileName)
        importFromFileName.clear();
    saveFilePathLineEdit->setText("FileFrom: " + QFileInfo(importfile).canonicalFilePath());

    importfile.close();
}

void MainWindow::sendFile()
{
    if((isOpenSerial == true) && (!importFromFileName.isEmpty()))
    {
        QString strSt;
        QFile importfile(importFromFileName);
        if(!importfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Import File Failed"), tr("Open file %1 failed, cannot save file\n%2")
                                 .arg(importFromFileName).arg(importfile.errorString()), QMessageBox::Ok);
            return;
        }
        if(importfile.size() > MAX_FILE_SIZE)
        {
            QMessageBox::critical(this, tr("File too large"), tr("File size is %1, Over Max file size %2")
                                  .arg(importfile.size()).arg(MAX_FILE_SIZE), QMessageBox::Ok);
            importfile.close();
            return;
        }
        else if(importfile.size() > TIP_FILE_SIZE)
        {
            if(QMessageBox::question(this, tr("File too large"), tr("File size too large, Confirm whether to open"),
                                  QMessageBox::Ok | QMessageBox::No) == QMessageBox::No)
            importfile.close();
            return;
        }

        QTextStream in(&importfile);
        //sendEdit->setText(in.readAll());
        strSt = in.readAll();
        myCom->write(strSt.toLocal8Bit());
        qDebug() << strSt;

        sendCount += (unsigned int)strSt.count();
        sendCountlabel->setText(tr("Send: %1 Byte ").arg(sendCount));
        importfile.close();
    }
}

void MainWindow::SendSerialData()
{
    if(isOpenSerial)
    {
        if(autoSendCKx->isChecked()) //check AutoSend
        {
            autoSendTimer->start();
            qDebug() << "start autoSend open";
        }
        serialWriteDate();        
    }
}

void MainWindow::openWave()
{
    if(isShowWave == false)
    {
        showWaveBtn->setText(tr("Close Waveform"));
        isShowWave = true;
        paintWave->show();
    }
    else
    {
        showWaveBtn->setText(tr("Show Waveform"));
        isShowWave = false;
        paintWave->hide();
    }
}

void MainWindow::serialWriteLineData()
{
    QString sendBuff = sendLine->text();//  ->toPlainText();
    QByteArray sendHexBuff;

    if(sendBuff.isEmpty())
    {
        sendLine->setFocus();
        return;
    }
    if(!myCom->isOpen())
        return;

    sendHexBuff = sendBuff.toLocal8Bit();

    if(sendNewLineCKx->isChecked())
    {
#ifdef Q_OS_WIN
        sendHexBuff.append("\r\n");
#elif defined (Q_OS_LINUX)
        sendHexBuff.append('\n');
#endif
    }

    myCom->write(sendHexBuff);
    //qDebug() << "Send " << s_sendCnt << sendHexBuff.size() << "Bytes ->" << sendHexBuff;
    sendCount += (unsigned int)sendHexBuff.size();
    sendCountlabel->setText(tr("Send: %1 Byte ").arg(sendCount));
}

