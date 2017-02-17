#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserial/qextserialport.h"
#include "receivethread.h"
#include "paintwave.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QTextEdit;
class QLineEdit;
class QSpinBox;
QT_END_NAMESPACE

//serialPort Out Time
#define TIME_OUT        10
#define TIP_FILE_SIZE   5000
#define MAX_FILE_SIZE   10000
#define MIN_AUTO_TIME   50
#define MAX_AUTO_TIME   60000

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *, QEvent *);
    void closeEvent(QCloseEvent *);

protected slots:
    void setTime();
    void serialWriteDate();
    void serialWriteLineData();
    void serialReadDate(const char *);
    void changeAutoSendTime(int);
    void clearReceiveArea();
    void clearSendArea();
    void clearStatusArea();
    void saveToFile();
    void sendFile();
    void openFileAPath();
    void SendSerialData();
    void openWave();
    //void changeUseProtocol();

private:
    void initUI(QWidget *);
    void filePortParameters();
    void initConnect();
    void openSerialPort();
    void setOpenObjectStatus(bool sta);
    void isAutoSend();

private:
    unsigned int sendCount = 0;
    unsigned int receiveCount = 0;
    QextSerialPort *myCom;
    bool isOpenSerial = false;
    bool isShowWave = false;
    bool isSaveFile = false;
    QByteArray dataBuff;
    ReceiveThread *serialThread;
    PaintWave *paintWave;
    QString writeToFileName;
    QString importFromFileName;
    QTimer *timerShow;
    QTimer *autoSendTimer;


private:
    QTextEdit *receiveEdit;
    QTextEdit *sendEdit;
    QGroupBox *receiveDataGBx;
    QGroupBox *portSelectGBx;
    QGroupBox *sendDataGBx;
    QGroupBox *otherInfoGBx;

    QComboBox *serialPortCBx;
    QComboBox *baudRateCBx;
    QComboBox *stopBitsCBX;
    QComboBox *dataBitsCBx;
    QComboBox *parityCBx;
    QComboBox *flowControlCBx;
    //---QComboBox *headPtlCBx;
    //---QComboBox *tailPtlCBx;
    QLabel *baudRatelabel;
    QLabel *stopBitslabel;
    QLabel *dataBitslabel;
    QLabel *paritylabel;
    QLabel *flowControllabel;
    //---QLabel *ptllabel;
    QPushButton *openPortBtn;
    QPushButton *showWaveBtn;
    QPushButton *sendPortBtn;
    QPushButton *saveFileBtn;
    QPushButton *importFileBtn;
    QPushButton *openFileBtn;
    QPushButton *clearReceiveDataBtn;
    QPushButton *clearSendDataBtn;
    QPushButton *clearStatusCntBtn;
    QCheckBox *sendNewLineCKx;
    QCheckBox *hexSendCKx;
    QCheckBox *hexReceiveCKx;
    QCheckBox *autoSendCKx;
    //---QCheckBox *useProtocolCKx;
    QSpinBox *autoSendTimeSPx;
    QLineEdit *saveFilePathLineEdit;
    QLineEdit *sendLine;
    QPushButton *sendLineBtn;

    QLabel *portStatuslabel;
    QLabel *sendCountlabel;
    QLabel *ReceiveCountlabel;
    QLabel *currentTimelabel;
};

#endif // MAINWINDOW_H
