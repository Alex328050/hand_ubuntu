#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QSlider>
#include <QLabel>
#include <QProcess>

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

#define ID1MAX 1760
#define ID1MIN 1370
#define ID2MAX 1740
#define ID2MIN 1122
#define ID3MAX 1350
#define ID3MIN 170
#define ID4MAX 1860
#define ID4MIN 1512
#define ID5MAX 1411
#define ID5MIN 260
#define ID6MAX 1870
#define ID6MIN 1415
#define ID7MAX 1750
#define ID7MIN 500
#define ID8MAX 1780
#define ID8MIN 1390
#define ID9MAX 1570
#define ID9MIN 450
#define ID10MAX 1400
#define ID10MIN 990

// #define Servo1Start 118
// #define Servo2Start 83
// #define Servo3Start 69
// #define Servo4Start 61
// #define Servo5Start 65

#define Servo1Start 0
#define Servo2Start 45
#define Servo3Start 64
#define Servo4Start 92
#define Servo5Start 65

// #define wristStartPos 500
#define wristStartPos 800
#define wristStep 25

extern uint16_t handMax[10];
extern uint16_t handMin[10];
extern float ADCValue[5];

typedef enum
{
    WIRE,
    WIRELESS,
    STORED_ACTION,
    CAMERA,
    USER_ACTION,
    GOLVE,
    ROCK,
    PIANO,
    OBJECT,
}MODE;

typedef enum
{
    NOACTION,
    RESUMEACTION,
    OK,
    YEAH,
    WRISTLEFTRIGHT,
    WRISTFRONTBACK,
    SWING,
    POINTFINGER,
    WIDEANDSHRINK,
    ACTION33,
    BACKMOVE,
    PIANOACTION,
}ACTION;
extern ACTION action;
extern MODE mode;

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
    void UR5Connect();
    void UR5Receive();
    void UR5Start();
    void ActionTimer();

    void on_modeWire_clicked();
    void on_modeWireless_clicked();
    void on_modeCamera_clicked();
    void on_modeRock_clicked();

    void on_modeGolve_clicked();
    void on_golveAdjustButton_clicked();

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

    void on_actionOK_clicked();
    void on_actionYEAH_clicked();
    void on_actionWRISTLEFTANDRIGHT_clicked();
    void on_actionWRISTFRONTANDBACK_clicked();
    void on_actionSWING_clicked();
    void on_actionWIDEANDSHRINK_clicked();
    void on_actionPOINTFINGER_clicked();
    void on_actionBACKMOVE_clicked();
    void on_actionACTION33_clicked();
    void on_actionRESUME_clicked();

    void on_modePiano_clicked();

    void on_modeObject_clicked();

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
    QTimer* myTimerAction;
    QTimer* myTimerUR5Receive;
    QProcess* tcpProcess;
    QVector<QSlider*> allSlider;
    QVector<QLabel*> allLabel;
};
#endif // WIDGET_H
