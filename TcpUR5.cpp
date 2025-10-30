#include <QTcpSocket>
#include <QTimer>
#include "ui_widget.h"
#include "widget.h"
#include <QWidget>

void Widget::UR5Connect()
{
    disconnect(UR5Socket, SIGNAL(readyRead()), nullptr, nullptr);
    connect(UR5Socket, SIGNAL(readyRead()), this, SLOT(UR5Receive()));
}

void Widget::UR5Start()
{
    if (UR5Socket->state() == QTcpSocket::UnconnectedState) UR5Socket->connectToHost("127.0.0.1", 8);
    connect(UR5Socket, SIGNAL(connected()), this, SLOT(UR5Connect()));
}

void Widget::UR5Receive()
{
    if (action == NOACTION && mode == OBJECT)
    {

    }
    else if (action == NOACTION && mode == PIANO)
    {

    }
}

// void Widget::on_modePiano_clicked()
// {
//     action = NOACTION;
//     mode = PIANO;
//     ui->modeLabel->clear();
//     ui->modeLabel->setText(QString("当前模式：图像识别模式"));
//     if (myTimerAction->isActive()) myTimerAction->stop();
//     myTimerAction = new QTimer(this);
//     connect(myTimerAction, &QTimer::timeout, this, &Widget::ActionTimer);
//     myTimerAction->start(300);
//     //myTimerUR5Receive->start(10);
// }

void Widget::on_modeObject_clicked()
{
    action = NOACTION;
    mode = OBJECT;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：图像识别模式"));
    myTimerUR5Receive->start(10);
}
// void Widget::armMove(double mx, double my, double mz, double mrx, double mry, double mrz)
// {
//     QString x, y, z, rx, ry, rz;
//     x = QString::number(mx, 'f', 6); //m
//     y = QString::number(my, 'f', 6);
//     z = QString::number(mz, 'f', 6);
//     rx = QString::number(mrx,'f',6); //rad
//     ry = QString::number(mry,'f',6);
//     rz = QString::number(mrz,'f',6);

//     QString txMessage = QString("movel(p[%1,%2,%3,%4,%5,%6],a=0.12,v=0.16,t=0,r=0)\n").arg(x, y, z, rx, ry, rz);
//     QByteArray txData = txMessage.toUtf8(); //QString编码格式为UTF-16，需转为UTF8的QBytearray后发送

//     UR5Socket->write(txData);
//     UR5Socket->flush();
// }
