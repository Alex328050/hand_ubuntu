#include "widget.h"
#include <QUdpSocket>
#include "myInferior.h"
#include <QMessageBox>
#include <QTimer>

#define ID1ratio 97
#define ID2ratio 51
#define ID3ratio 91
#define ID4ratio 78
#define ID5ratio 100
#define ID6ratio 67
#define ID7ratio 94
#define ID8ratio 79
#define ID9ratio 82
#define ID10ratio 81
#define ID11ratio 90
#define ID12ratio 90
#define ID13ratio 90
#define ID14ratio 90
#define ID15ratio 90

int adjustProgress = 0;
int isDone = 0;
double allRatio[15] = {ID1ratio,ID2ratio,ID3ratio,ID4ratio,ID5ratio,ID6ratio,ID7ratio,ID8ratio,ID9ratio,ID10ratio,ID11ratio,ID12ratio,ID13ratio,ID14ratio,ID15ratio};

void Widget::on_modeGolve_clicked()
{
    action = NOACTION;
    mode = GOLVE;
    golveSendSocket->writeDatagram("s", QHostAddress("127.0.0.1"), 1349);
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：数据手套模式"));
    system("cd .\\handglove\\study_2025-8-19\\ & start python .\\main.py");
    golveSocket->abort();
    golveSocket->bind(1348);
    disconnect(golveSocket, SIGNAL(readyRead()), nullptr, nullptr);
    connect(golveSocket, SIGNAL(readyRead()), this, SLOT(golveReceive()));
    golveOrderSocket->abort();
    golveOrderSocket->bind(1350);
    disconnect(golveOrderSocket, SIGNAL(readyRead()), nullptr, nullptr);
    connect(golveOrderSocket, SIGNAL(readyRead()), this, SLOT(golveOrderReceive()));
}

void Widget::golveReceive()
{
    if (action == NOACTION && mode == GOLVE)
    {
        while (golveSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(golveSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            golveSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
            QString rawData = QString::fromUtf8(datagram);
            rawData = rawData.mid(3);
            QStringList numList = rawData.split(',');
            QVector<double> result;
            for (const QString& str : numList)
            {
                result.push_back(str.toDouble());
            }
            QVector<double> angle;
            angle.push_back(result[0]);
            angle.push_back(result[10]);
            angle.push_back(result[1]);
            angle.push_back(-result[11]);
            angle.push_back(result[2]);
            angle.push_back(-result[12]);
            angle.push_back(result[3]);
            angle.push_back(-result[13]);
            angle.push_back(result[4]);
            angle.push_back(-result[14]);
            angle.push_back(result[5]);
            angle.push_back(result[6]);
            angle.push_back(result[7]);
            angle.push_back(result[8]);
            angle.push_back(result[9]);
            for (int i = 0; i < 10; ++i)
            {
                angle[i] = angle[i] * 90.0 / allRatio[i];
                if (angle[i] < 0) angle[i] = 0;
                else if (angle[i] > 90) angle[i] = 90;
            }
            for (int i = 0; i < 15; ++i)
                qDebug() << angle[i] << '\t';
            qDebug() << '\n';
            for (int i = 0; i < 10; ++i)
            {
                handData[i] = static_cast<int>(handMax[i] - (angle[i]/90.0) * (handMax[i]-handMin[i]));
                qDebug() << handData[i] << ",";
            }
            handData[12] = static_cast<int>(angle[10] / 80.0 * 120.0);
            if (handData[12] >= 120) handData[12] = 120;
            handData[13] = Servo2Start - angle[11];
            handData[14] = Servo3Start - angle[12];
            handData[15] = Servo4Start - angle[13];
            handData[16] = Servo5Start - angle[14];
        }
    }
}

void Widget::golveOrderReceive()
{
    if (action == NOACTION && mode == GOLVE)
    {
        while (golveOrderSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(golveOrderSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            golveOrderSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
            QString rawData = QString::fromUtf8(datagram);
            if (rawData == QString("done!")) isDone = 1;
            qDebug() << rawData;
        }
    }
}

void Widget::on_golveAdjustButton_clicked()
{
    if (action == NOACTION && mode == GOLVE)
    {
        if (adjustProgress == 0)
        {
            adjustProgress = 1;
            golveSendSocket->writeDatagram("a", QHostAddress("127.0.0.1"), 1349);
            golveSendSocket->writeDatagram("t", QHostAddress("127.0.0.1"), 1349);
        }
        else
        {
            adjustProgress = 0;
            golveSendSocket->writeDatagram("d", QHostAddress("127.0.0.1"), 1349);
            QTimer::singleShot(200, [=]() {
                if (isDone == 1)
                {
                    QMessageBox::information(this, "提示", "数据手套校准完成！");
                    isDone = 0;
                }
                else QMessageBox::critical(this, "提示", "数据手套校准失败！");
            });
        }
    }
    else QMessageBox::critical(this, "提示", "请先切换到数据手套模式！");
}
