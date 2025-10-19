#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QSlider>
#include <QLabel>

// #define ID1MAX 1850
// #define ID1MIN 700
// #define ID2MAX 2000
// #define ID2MIN 1350
// #define ID3MAX 1800
// #define ID3MIN 550
// #define ID4MAX 1950
// #define ID4MIN 1450
// #define ID5MAX 1800
// #define ID5MIN 500
// #define ID6MAX 1650
// #define ID6MIN 1100
// #define ID7MAX 1600
// #define ID7MIN 450
// #define ID8MAX 1850
// #define ID8MIN 1250
// #define ID9MAX 1850
// #define ID9MIN 500
// #define ID10MAX 1850
// #define ID10MIN 1350

#define ID1MAX 1671
#define ID1MIN 441
#define ID2MAX 1482
#define ID2MIN 310
#define ID3MAX 1950
#define ID3MIN 706
#define ID4MAX 1790
#define ID4MIN 857
#define ID5MAX 1419
#define ID5MIN 539
#define ID6MAX 1654
#define ID6MIN 637
#define ID7MAX 1713
#define ID7MIN 514
#define ID8MAX 1944
#define ID8MIN 939
#define ID9MAX 1237
#define ID9MIN 155
#define ID10MAX 1692
#define ID10MIN 706

#define Servo1Start 110
#define Servo2Start 81
#define Servo3Start 66
#define Servo4Start 65
#define Servo5Start 48

// #define wristStartPos 500
#define wristStartPos 800
#define wristStep 25

extern uint16_t handMax[10];
extern uint16_t handMin[10];
extern float ADCValue[5];
extern int action;
extern int mode;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_openButton_clicked();

    void on_closeButton_clicked();

    void updateCOM();
    void serialTransmit();
    void serialReceive();
    void tcpReceive();
    void tcpConnect();
    void tcpStart();
    void golveReceive();
    void golveOrderReceive();
    void UR5Connect(QString ipAddr);
    void armMove(double mx, double my, double mz, double mrx, double mry, double mrz);

    void on_action1_clicked();

    void on_action2_clicked();

    void on_action3_clicked();

    void on_action4_clicked();

    void on_modeWire_clicked();

    void on_modeWireless_clicked();

    void on_modeCamera_clicked();

    void on_action5_clicked();

    void on_action6_clicked();

    void on_modeGolve_clicked();

    void on_resume_clicked();

    void on_actionSlider1_valueChanged(int value);

    void on_actionSlider2_valueChanged(int value);

    void on_actionSlider3_valueChanged(int value);

    void on_actionSlider4_valueChanged(int value);

    void on_actionSlider5_valueChanged(int value);

    void on_actionSlider6_valueChanged(int value);

    void on_actionSlider7_valueChanged(int value);

    void on_actionSlider8_valueChanged(int value);

    void on_actionSlider9_valueChanged(int value);

    void on_actionSlider10_valueChanged(int value);

    void on_actionSlider11_valueChanged(int value);

    void on_actionSlider12_valueChanged(int value);

    void on_actionSlider13_valueChanged(int value);

    void on_actionSlider14_valueChanged(int value);

    void on_actionSlider15_valueChanged(int value);

    void on_actionSlider16_valueChanged(int value);

    void on_actionSlider17_valueChanged(int value);

    void on_actionSlider18_valueChanged(int value);

    void on_modeEveryAction_clicked();

    void on_addActionButton_clicked();

    void on_deleteActionButton_clicked();

    void on_actButton_clicked();

    void on_golveAdjustButton_clicked();

    void on_action7_clicked();
    void Action7();

    void on_action8_clicked();

    void on_modeRock_clicked();

private:
    Ui::Widget *ui;
    QSerialPort* serialPort;
    QTcpSocket* tcpSocket;
    QTcpSocket* UR5Socket;
    QUdpSocket* golveSocket;
    QUdpSocket* golveSendSocket;
    QUdpSocket* golveOrderSocket;
    QTimer* myTimerTcpReceive;
    QTimer* myTimerGolveReceive;
    QTimer* myTimerAction7;
    QVector<QSlider*> allSlider;
    QVector<QLabel*> allLabel;
};
#endif // WIDGET_H
