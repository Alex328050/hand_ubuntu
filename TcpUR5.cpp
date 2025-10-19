#include "widget.h"
#include <QTcpSocket>

void Widget::UR5Connect(QString ipAddr)
{
    if (UR5Socket->state() == QTcpSocket::UnconnectedState)
    {
        UR5Socket->connectToHost(ipAddr, 30001); //连接至服务器，端口30002
    }
}

void Widget::armMove(double mx, double my, double mz, double mrx, double mry, double mrz)
{
    QString x, y, z, rx, ry, rz;
    x = QString::number(mx, 'f', 6); //m
    y = QString::number(my, 'f', 6);
    z = QString::number(mz, 'f', 6);
    rx = QString::number(mrx,'f',6); //rad
    ry = QString::number(mry,'f',6);
    rz = QString::number(mrz,'f',6);

    QString txMessage = QString("movel(p[%1,%2,%3,%4,%5,%6],a=0.12,v=0.16,t=0,r=0)\n").arg(x, y, z, rx, ry, rz);
    QByteArray txData = txMessage.toUtf8(); //QString编码格式为UTF-16，需转为UTF8的QBytearray后发送

    UR5Socket->write(txData);
    UR5Socket->flush();
}
