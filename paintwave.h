#ifndef PAINTWAVE_H
#define PAINTWAVE_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QPushButton;
class QSpinBox;
class QPushButton;
QT_END_NAMESPACE

#define PI 3.14159265
#define WAVE_WIDTH  730
#define WAVE_HEIGHT 500
/*
 * Pinter to QLabel
 *
*/
class PaintLabel : public QLabel
{
    Q_OBJECT

public:
    PaintLabel(QWidget *parent=0);
    ~PaintLabel();

    void addItemToList(float);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void toShowGrad();
    void gradNumChange(int);
    void gradInvChange(int);
    void yMaxValueChange(int);
    void goalValueChange(int);
    void checkWaveChange();
    void clearWaveData();

private:
    void showGrid();
    void setXYCord();
    //void RealTimeWave();
    void realTimerWave();
    void historyTimeWave();
//    void testAddItem();

private:
    QPainter *painter;
    QRect rect;
    QPointF initPoint;
    QPointF point[2];
    QPointF newVal;
    QList<QPointF> historyValList;

    int gridnum;
    int pointInterval;
    int halfHight;
    int yMaxValue;
    int goalValue;
    bool isShowGrid;
    bool isCheckWave;
    int startPos;
    int moveDst;
    volatile int hstyDst;
    int leftPos;
    int rightPos;
};


/*
 *  PaintWave QWidget
 *
*/
class PaintWave : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWave(QWidget *parent = 0);
    ~PaintWave();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void closeWaveSignal();
    void sendShowGradSig();
    void sendCheckWaveSig();

public slots:
    void handleWaveItem(const char *);
    void sendCloseWave();
    void switchShowWave();
    void switchShowGrad();
    void switchChechWave();


private:
    void initGUI();

private:
    bool isShowWave;
    bool isShowGrad;
    bool isCheckWave;

private:
    PaintLabel *paintLbl;
    QGroupBox *opGB;
    QGroupBox *spGB;
    QPushButton *showGradBtn;
    QLabel *gradNumLabel;
    QLabel *pointInvLabel;
    QLabel *yLabel;
    QLabel *xLabel;
    QLabel *yMaxLabel;
    QLabel *goalLabel;
    QSpinBox *gradNumSpx;
    QSpinBox *gradInvSpx;
    QSpinBox *yMaxValSpx;
    QSpinBox *goalLineSpx;
    QPushButton *closeWaveBtn;
    QPushButton *stopShowBtn;
    QPushButton *checkWaveBtn;
    QPushButton *clearWaveBtn;
};

#endif // PAINTWAVE_H
