#include "widget.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QTimer>
#include "myInferior.h"

void Widget::on_modeCamera_clicked()
{
    mode = CAMERA;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：图像识别模式"));
    action = NOACTION;
    system("bash -c 'cd ./handpose && source ./.venv/bin/activate && python ./dexhand-ble.py'");
    myTimerTcpReceive->start(10);
}

void Widget::on_modeRock_clicked()
{
    mode = ROCK;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：猜拳模式"));
    action = NOACTION;
    system("bash -c 'cd ./handpose && source ./.venv/bin/activate && python ./dexhand-ble.py'");
    myTimerTcpReceive->start(10);
}

void Widget::tcpReceive()
{
    if (action == NOACTION && mode == CAMERA)
    {
        QByteArray tcpBuffer;
        tcpBuffer.append(tcpSocket->readAll());
        QJsonParseError parseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(tcpBuffer, &parseError);
        if (parseError.error == QJsonParseError::NoError)
        {
            QJsonObject jsonObject = jsonDocument.object();
            QJsonArray angleArray = jsonObject["angles"].toArray();
            QVector<double> angle;
            for (int i = 0; i < angleArray.size(); ++i) angle.append(angleArray.at(i).toDouble());
            for (int i = 2; i < 12; i += 3) angle[i] /= 2;
            QVector<double> sortAngle;
            sortAngle.append(angle[14]);
            sortAngle.append(angle[12]);
            sortAngle.append(angle[2]);
            sortAngle.append(angle[0]);
            sortAngle.append(angle[5]);
            sortAngle.append(angle[3]);
            sortAngle.append(angle[8]);
            sortAngle.append(angle[6]);
            sortAngle.append(angle[11]);
            sortAngle.append(angle[9]);
            sortAngle.append(angle[16]);
            sortAngle.append(angle[17]);
            sortAngle.append(angle[15]);
            sortAngle.append(angle[1]);
            sortAngle.append(angle[7]);
            sortAngle.append(angle[10]);
            for (int i = 0; i < 16; ++i)
            {
                if (sortAngle[i] > 90) sortAngle[i] = 90;
            }
            if (sortAngle[1] > 45) sortAngle[1] = 45;
            //qDebug() << sortAngle;
            for (int i = 0; i < 10; ++i)
            {
                handData[i] = static_cast<int>(handMax[i] - (sortAngle[i]/90) * (handMax[i]-handMin[i]));
                //qDebug() << handData[i] << ",";
            }
            handData[1] = static_cast<int>(handMax[1] - (sortAngle[1]/45) * (handMax[1]-handMin[1]));
            handData[12] = static_cast<int>(180 - (180 - 20) * (sortAngle[1]/45.0));
            //qDebug("\n");
            tcpBuffer.clear();
        }
        else if (tcpBuffer[0] != '{') tcpBuffer.clear();
    }
    else if (action == NOACTION && mode == ROCK)
    {
        QByteArray tcpBuffer;
        tcpBuffer.append(tcpSocket->readAll());
        QJsonParseError parseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(tcpBuffer, &parseError);
        if (parseError.error == QJsonParseError::NoError)
        {
            QJsonObject jsonObject = jsonDocument.object();
            QString rock = jsonObject["pose"].toString();
            qDebug() << rock;
            if (rock == "rock")
            {
                for (uint8_t i = 0; i < 10; ++i) handData[i] = handMax[i];
                handData[13] = Servo2Start;
                handData[14] = Servo3Start;
            }
            else if (rock == "scissors")
            {
                for (uint8_t i = 0; i < 10; ++i) handData[i] = handMin[i];
                handData[0] += 400;
                handData[1] += 600;
                handData[13] = Servo2Start;
                handData[14] = Servo3Start;
            }
            else if (rock == "paper")
            {
                for (uint8_t i = 0; i < 10; ++i) handData[i] = handMin[i];
                handData[0] += 400;
                handData[1] += 600;
                for (uint8_t i = 2; i < 6; ++i) handData[i] = handMax[i];
                handData[13] = Servo2Start + 10;
                handData[14] = Servo3Start - 10;
            }
        }
    }
}

void Widget::tcpConnect()
{
    disconnect(tcpSocket, SIGNAL(readyRead()), nullptr, nullptr);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReceive()));
}

void Widget::tcpStart()
{
    if (tcpSocket->state() == QTcpSocket::UnconnectedState) tcpSocket->connectToHost("127.0.0.1", 8666);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(tcpConnect()));
}
