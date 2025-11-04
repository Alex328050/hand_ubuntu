#include "widget.h"
#include "ui_widget.h"
#include "myInferior.h"

#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <cstdlib>
#include <QUdpSocket>
#include "handData.cpp"

int isInferiorReadyFlag = 0;
ACTION action = NOACTION;
MODE mode = STORED_ACTION;
int sinCount = 0;
int wristCount = wristStep;
QByteArray receiveBuffer;
uint16_t handMax[10] = {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX};
uint16_t handMin[10] = {ID1MIN, ID2MIN, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN};
float ADCValue[5] = {0};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QStringList serialNamePort;

    ui->setupUi(this);
    this->setWindowTitle("天工巧手");

    /* 创建一个串口对象 */
    serialPort = new QSerialPort(this);
    tcpSocket = new QTcpSocket(this);
    UR5Socket = new QTcpSocket(this);
    golveSocket = new QUdpSocket(this);
    golveSendSocket = new QUdpSocket(this);
    golveOrderSocket = new QUdpSocket(this);

    /* 搜索所有可用串口 */
    foreach (const QSerialPortInfo &inf0, QSerialPortInfo::availablePorts()) {
        serialNamePort<<inf0.portName();
    }
    serialNamePort.sort();
    ui->serialBox->addItems(serialNamePort);

    QTimer* myTimerCOM = new QTimer(this);
    connect(myTimerCOM, &QTimer::timeout, this, &Widget::updateCOM);
    myTimerCOM->start(100);

    QTimer* myTimerTransmit = new QTimer(this);
    connect(myTimerTransmit, &QTimer::timeout, this, &Widget::serialTransmit);
    myTimerTransmit->start(5);

    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);

    myTimerTcpReceive = new QTimer(this);
    connect(myTimerTcpReceive, &QTimer::timeout, this, &Widget::tcpStart);
    tcpProcess = new QProcess(this);

    myTimerUR5Receive = new QTimer(this);
    connect(myTimerUR5Receive, &QTimer::timeout, this, &Widget::UR5Start);

    allSlider.push_back(ui->actionSlider1);
    allSlider.push_back(ui->actionSlider2);
    allSlider.push_back(ui->actionSlider3);
    allSlider.push_back(ui->actionSlider4);
    allSlider.push_back(ui->actionSlider5);
    allSlider.push_back(ui->actionSlider6);
    allSlider.push_back(ui->actionSlider7);
    allSlider.push_back(ui->actionSlider8);
    allSlider.push_back(ui->actionSlider9);
    allSlider.push_back(ui->actionSlider10);
    allSlider.push_back(ui->actionSlider11);
    allSlider.push_back(ui->actionSlider12);
    allSlider.push_back(ui->actionSlider13);
    allSlider.push_back(ui->actionSlider14);
    allSlider.push_back(ui->actionSlider15);
    allSlider.push_back(ui->actionSlider16);
    allSlider.push_back(ui->actionSlider17);
    allSlider.push_back(ui->actionSlider18);

    allLabel.push_back(ui->actionLabel1);
    allLabel.push_back(ui->actionLabel2);
    allLabel.push_back(ui->actionLabel3);
    allLabel.push_back(ui->actionLabel4);
    allLabel.push_back(ui->actionLabel5);
    allLabel.push_back(ui->actionLabel6);
    allLabel.push_back(ui->actionLabel7);
    allLabel.push_back(ui->actionLabel8);
    allLabel.push_back(ui->actionLabel9);
    allLabel.push_back(ui->actionLabel10);
    allLabel.push_back(ui->actionLabel11);
    allLabel.push_back(ui->actionLabel12);
    allLabel.push_back(ui->actionLabel13);
    allLabel.push_back(ui->actionLabel14);
    allLabel.push_back(ui->actionLabel15);
    allLabel.push_back(ui->actionLabel16);
    allLabel.push_back(ui->actionLabel17);
    allLabel.push_back(ui->actionLabel18);

    for (int i = 0; i < 17; ++i) allSlider[i]->setValue(handData[i]);
    for (int i = 0; i < 10; ++i) allSlider[i]->setMinimum(handMin[i]);
    for (int i = 0; i < 18; ++i)
    {
        allLabel[i]->clear();
        allLabel[i]->setNum(allSlider[i]->value());
    }

}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateCOM()
{
    QStringList serialNamePort;
    QStringList boxName;
    QStringList addName;
    QStringList deleteName;
    for (int i = 0; i < ui->serialBox->count(); ++i) boxName << ui->serialBox->itemText(i);
    foreach (const QSerialPortInfo &inf0, QSerialPortInfo::availablePorts()) {
        serialNamePort << inf0.portName();
    }
    serialNamePort.sort();
    boxName.sort();
    foreach (const QString &str, serialNamePort) {
        bool isStrDifferent = true;
        foreach (const QString &strInBox, boxName) {
            if (str == strInBox) isStrDifferent = false;
        }
        if (isStrDifferent) addName << str;
    }
    foreach (const QString &strInBox, boxName) {
        bool isStrInBoxDifferent = true;
        foreach (const QString &str, serialNamePort) {
            if (strInBox == str) isStrInBoxDifferent = false;
        }
        if (isStrInBoxDifferent) deleteName << strInBox;
    }
    foreach (const QString &addStr, addName)
    {
        int insertPos = 0;
        for(; insertPos < ui->serialBox->count() && addStr > ui->serialBox->itemText(insertPos); ++insertPos);
        ui->serialBox->insertItem(insertPos, addStr);
    }
    foreach (const QString &deleteStr, deleteName) {
        for (int i = 0; i < ui->serialBox->count(); ++i)
        {
            if (deleteStr == ui->serialBox->itemText(i)) ui->serialBox->removeItem(i);
        }
    }
    ui->ADCLabel1->setText(QString::number(ADCValue[0], 'f', 2));
    ui->ADCLabel2->setText(QString::number(ADCValue[1], 'f', 2));
    ui->ADCLabel3->setText(QString::number(ADCValue[2], 'f', 2));
    ui->ADCLabel4->setText(QString::number(ADCValue[3], 'f', 2));
    ui->ADCLabel5->setText(QString::number(ADCValue[4], 'f', 2));
}

void Widget::on_openButton_clicked()
{
    /* 串口设置 */
    serialPort->setPortName(ui->serialBox->currentText());
    serialPort->setBaudRate(ui->baudrateBox->currentText().toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);

    /* 打开串口提示框 */
    if (true == serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, "提示", "串口打开成功！");
        connect(serialPort, &QSerialPort::readyRead, this, &Widget::serialReceive);
    }
    else
    {
        QMessageBox::critical(this, "提示", "串口打开失败！");
    }
}

void Widget::on_closeButton_clicked()
{
    serialPort->close();
    QMessageBox::critical(this, "提示", "串口关闭！");
}

void Widget::serialTransmit()
{
    if (serialPort->isOpen())
    {
        if (isInferiorReadyFlag == 1)
        {
            switch (mode) {
            case WIRE:
                transmitData[0] = 0xFF;
                transmitData[1] = 0xFE;
                for (uint8_t i = 2; i < 2+34+1; ++i)
                {
                    transmitData[i] = -1;
                }
                transmitData[2+34+1] = calCheckSum(transmitData, 2+34+1+1);
                transmitData[2+34+1+1] = 0xFD;
                transmitData[2+34+1+2] = 0xFC;
                serialPort->write(transmitData, 40);
                break;
            case WIRELESS:
                transmitData[0] = 0xFF;
                transmitData[1] = 0xFE;
                for (uint8_t i = 2; i < 2+34+1; ++i)
                {
                    transmitData[i] = -2;
                }
                transmitData[2+34+1] = calCheckSum(transmitData, 2+34+1+1);
                transmitData[2+34+1+1] = 0xFD;
                transmitData[2+34+1+2] = 0xFC;
                serialPort->write(transmitData, 40);
                break;
            default:
                break;
            }
            if (mode == WIRE || mode == WIRELESS) mode = STORED_ACTION;
            else
            {
                uint8_t isClearFault = 0;
                switch (action) {
                case OK:
                    memcpy(handData, myhandDataOK, sizeof(handData));
                    break;
                case YEAH:
                    memcpy(handData, myhandDataYEAH, sizeof(handData));
                    break;
                case WRISTFRONTBACK:
                    handData[10] += wristCount;
                    handData[11] += wristCount;
                    if (handData[10] == 300) wristCount = wristStep;
                    else if (handData[10] == 1300) wristCount = -wristStep;
                    break;
                case WRISTLEFTRIGHT:
                    handData[10] += wristCount;
                    handData[11] += -wristCount;
                    if (handData[10] == 500) wristCount = wristStep;
                    else if (handData[10] == 1100) wristCount = -wristStep;
                    break;
                case SWING:
                    memcpy(handData, myhandDataSWING[handData_index], sizeof(handData));
                    break;
                case WIDEANDSHRINK:
                    memcpy(handData, myhandDataWIDEANDSHRINK[handData_index], sizeof(handData));
                    break;
                case POINTFINGER:
                    memcpy(handData, myhandDataPOINTFINGER[handData_index], sizeof(handData));
                    break;
                case BACKMOVE:
                    for (uint8_t i = 0; i < 5; ++i)
                    {
                        if (ADCValue[i] > 10)
                        {
                            handData[i*2] += (handMax[i*2]-handMin[i*2]) / 35;
                            handData[i*2+1] += (handMax[i*2+1]-handMin[i*2+1]) / 35;
                        }
                    }
                    for (uint8_t i = 0; i < 10; ++i) if (handData[i] >= handMax[i]) handData[i] = handMax[i];
                    break;
                case PIANOACTION:
                    memcpy(handData, myhandDataScrew[handData_index], sizeof(handData));
                    break;
                case ACTION33:
                    memcpy(handData, myhandDataACTION33[handData_index], sizeof(handData));
                    break;
                default:
                    break;
                }
                if (action == RESUMEACTION && mode == STORED_ACTION) isClearFault = 1;
                else isClearFault = 0;
                calTransmit(handData, isClearFault);
                serialPort->write(transmitData, 2+34+1+1+2);
                isInferiorReadyFlag = 0;
            }
        }
    }
}

void Widget::serialReceive()
{
    QByteArray buffer = serialPort->readAll();
    if (buffer.size() >= 1)
    {
        if (buffer[0] == -1)
        {
            receiveBuffer.clear();
            receiveBuffer.push_back(buffer);
        }
        else receiveBuffer.push_back(buffer);
    }
    if (receiveBuffer.size() >= 9)
    {
        if (receiveBuffer[0] == -1 && receiveBuffer[1] == -2 && receiveBuffer[2] == 'O' && receiveBuffer[3] == 'K')
        {
            isInferiorReadyFlag = 1;
            ADCValue[0] = (uint8_t)receiveBuffer[4] / 255.0 * 15.0;
            ADCValue[1] = (uint8_t)receiveBuffer[5] / 255.0 * 15.0;
            ADCValue[2] = (uint8_t)receiveBuffer[6] / 255.0 * 15.0;
            ADCValue[3] = (uint8_t)receiveBuffer[7] / 255.0 * 15.0;
            ADCValue[4] = (uint8_t)receiveBuffer[8] / 255.0 * 15.0;
        }
    }
    else isInferiorReadyFlag = 0;
}

void Widget::on_modeWire_clicked()
{
    mode = WIRE;
}

void Widget::on_modeWireless_clicked()
{
    mode = WIRELESS;
}

void Widget::on_actionOK_clicked()
{
    mode = STORED_ACTION;
    action = OK;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
}

void Widget::on_actionYEAH_clicked()
{
    mode = STORED_ACTION;
    action = YEAH;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
}

void Widget::on_actionWRISTLEFTANDRIGHT_clicked()
{
    mode = STORED_ACTION;
    action = WRISTLEFTRIGHT;
    for (uint8_t i = 0; i < 10; ++i) handData[i] = 2000;
    handData[10] = wristStartPos;
    handData[11] = wristStartPos;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
}

void Widget::on_actionWRISTFRONTANDBACK_clicked()
{
    mode = STORED_ACTION;
    action = WRISTFRONTBACK;
    for (uint8_t i = 0; i < 10; ++i) handData[i] = 2000;
    handData[10] = wristStartPos;
    handData[11] = wristStartPos;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
}

void Widget::on_actionSWING_clicked()
{
    mode = STORED_ACTION;
    action = SWING;
    handData_index = 0;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
    if (myTimerAction->isActive()) myTimerAction->stop();
    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
    myTimerAction->start(250);
}

void Widget::on_actionWIDEANDSHRINK_clicked()
{
    mode = STORED_ACTION;
    action = WIDEANDSHRINK;
    handData_index = 0;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
    if (myTimerAction->isActive()) myTimerAction->stop();
    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
    myTimerAction->start(500);
}

void Widget::on_actionPOINTFINGER_clicked()
{
    mode = STORED_ACTION;
    action = POINTFINGER;
    handData_index = 0;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
    if (myTimerAction->isActive()) myTimerAction->stop();
    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
    myTimerAction->start(900);
}

void Widget::on_actionBACKMOVE_clicked()
{
    mode = STORED_ACTION;
    action = BACKMOVE;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
    for (uint8_t i = 6; i < 10; ++i) handData[i] = handMin[i];
    // handData[2] = handMin[2];
    // handData[3] = handMin[3];
    //handData[1] += 200;
}

void Widget::on_actionACTION33_clicked()
{
    mode = STORED_ACTION;
    action = ACTION33;
    handData_index = 0;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
    if (myTimerAction->isActive()) myTimerAction->stop();
    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
    myTimerAction->start(1500);
}

void Widget::on_actionRESUME_clicked()
{
    mode = STORED_ACTION;
    action = RESUMEACTION;
    for (uint8_t i = 0; i < 10; ++i) handData[i] = handMax[i];
    handData[10] = wristStartPos;
    handData[11] = wristStartPos;
    handData[12] = Servo1Start;
    handData[13] = Servo2Start;
    handData[14] = Servo3Start;
    handData[15] = Servo4Start;
    handData[16] = Servo5Start;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：灵巧手自带动作模式"));
}

void Widget::ActionTimer()
{
    ++handData_index;
    if      (mode == STORED_ACTION && action == SWING) {if (handData_index > 9) handData_index = 0;}
    else if (mode == STORED_ACTION && action == POINTFINGER) {if (handData_index > 3) handData_index = 0;}
    else if (mode == STORED_ACTION && action == WIDEANDSHRINK) {if (handData_index > 1) handData_index = 0;}
    else if (mode == STORED_ACTION && action == ACTION33) {if (handData_index > 9) handData_index = 0;}
    else if (mode == STORED_ACTION && action == PIANOACTION) {if (handData_index > 2) handData_index = 0;}
}

void Widget::on_modePiano_clicked()
{
    handData_index = 0;
    action = PIANOACTION;
    mode = STORED_ACTION;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：图像识别模式"));
    if (myTimerAction->isActive()) myTimerAction->stop();
    myTimerAction = new QTimer(this);
    connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
    myTimerAction->start(100);
    //myTimerUR5Receive->start(10);
}
