#include "widget.h"
#include "myInferior.h"
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <fstream>

QVector<QVector<uint16_t>> allAction;

void Widget::on_actionSlider1_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[0] = value;
    allLabel[0]->clear();
    allLabel[0]->setNum(value);
}


void Widget::on_actionSlider2_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[1] = value;
    allLabel[1]->clear();
    allLabel[1]->setNum(value);
}


void Widget::on_actionSlider3_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[2] = value;
    allLabel[2]->clear();
    allLabel[2]->setNum(value);
}


void Widget::on_actionSlider4_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[3] = value;
    allLabel[3]->clear();
    allLabel[3]->setNum(value);
}


void Widget::on_actionSlider5_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[4] = value;
    allLabel[4]->clear();
    allLabel[4]->setNum(value);
}


void Widget::on_actionSlider6_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[5] = value;
    allLabel[5]->clear();
    allLabel[5]->setNum(value);
}


void Widget::on_actionSlider7_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[6] = value;
    allLabel[6]->clear();
    allLabel[6]->setNum(value);
}


void Widget::on_actionSlider8_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[7] = value;
    allLabel[7]->clear();
    allLabel[7]->setNum(value);
}


void Widget::on_actionSlider9_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[8] = value;
    allLabel[8]->clear();
    allLabel[8]->setNum(value);
}


void Widget::on_actionSlider10_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[9] = value;
    allLabel[9]->clear();
    allLabel[9]->setNum(value);
}


void Widget::on_actionSlider11_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[10] = value;
    allLabel[10]->clear();
    allLabel[10]->setNum(value);
}


void Widget::on_actionSlider12_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[11] = value;
    allLabel[11]->clear();
    allLabel[11]->setNum(value);
}


void Widget::on_actionSlider13_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[12] = value;
    allLabel[12]->clear();
    allLabel[12]->setNum(value);
}


void Widget::on_actionSlider14_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[13] = value;
    allLabel[13]->clear();
    allLabel[13]->setNum(value);
}


void Widget::on_actionSlider15_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[14] = value;
    allLabel[14]->clear();
    allLabel[14]->setNum(value);
}


void Widget::on_actionSlider16_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[15] = value;
    allLabel[15]->clear();
    allLabel[15]->setNum(value);
}


void Widget::on_actionSlider17_valueChanged(int value)
{
    if (mode == USER_ACTION) handData[16] = value;
    allLabel[16]->clear();
    allLabel[16]->setNum(value);
}


void Widget::on_actionSlider18_valueChanged(int value)
{
    //if (mode == 5) handData[17] = value;
    allLabel[17]->clear();
    allLabel[17]->setNum(value);
}

void Widget::on_modeEveryAction_clicked()
{
    action = NOACTION;
    mode = USER_ACTION;
    ui->modeLabel->clear();
    ui->modeLabel->setText(QString("当前模式：用户自定义动作模式"));
    for (int i = 0; i < 17; ++i) handData[i] = allSlider[i]->value();
}

void Widget::on_addActionButton_clicked()
{
    if (mode == USER_ACTION)
    {
        QVector<uint16_t> temp;
        for (int i = 0; i < 17; ++i) temp.push_back(handData[i]);
        allAction.push_back(temp);

        std::ofstream myfile;
        myfile.open("action.txt", std::ios::app);
        myfile << "{";
        for (uint8_t i = 0; i < 10; ++i) myfile << "ID" << std::to_string(i+1) << "MAX-" << ((temp[i] <= handMax[i])?(handMax[i] - temp[i]):handMax[i]) << ", ";
        myfile << "wristStartPos-(" << wristStartPos - temp[10] << "), ";
        myfile << "wristStartPos-(" << wristStartPos - temp[11] << "), ";
        myfile << "Servo1Start-(" << Servo1Start - temp[12] << "), ";
        myfile << "Servo2Start-(" << Servo2Start - temp[13] << "), ";
        myfile << "Servo3Start-(" << Servo3Start - temp[14] << "), ";
        myfile << "Servo4Start-(" << Servo4Start - temp[15] << "), ";
        myfile << "Servo5Start-(" << Servo5Start - temp[16] << ")},\n";
        myfile.close();

        QString str;
        if (ui->actionBox->count() == 0)
        {
            str = QString("动作1");
        }
        else
        {
            str = QString("动作%1").arg(QString(QRegularExpressionMatch(QRegularExpression("(动作)(\\d+)").match(ui->actionBox->itemText(ui->actionBox->count()-1))).captured(2)).toInt()+1);
        }
        ui->actionBox->addItem(str);
    }
    else QMessageBox::critical(this, "提示", "请先切换到用户定义动作模式！");
}

void Widget::on_deleteActionButton_clicked()
{
    if (mode == USER_ACTION)
    {
        int index = ui->actionBox->currentIndex();
        ui->actionBox->removeItem(index);
        if (allAction.empty() != true) allAction.erase(allAction.begin()+index);
    }
    else QMessageBox::critical(this, "提示", "请先切换到用户定义动作模式！");
}

void Widget::on_actButton_clicked()
{
    if (mode == USER_ACTION)
    {
        if (ui->actionBox->count() != 0)
        {
            for (int i = 0; i < 17; ++i)
            {
                handData[i] = allAction[ui->actionBox->currentIndex()][i];
                allSlider[i]->setValue(handData[i]);
                allLabel[i]->clear();
                allLabel[i]->setNum(handData[i]);
            }
        }
        else QMessageBox::critical(this, "提示", "请先添加动作！");
    }
    else QMessageBox::critical(this, "提示", "请先切换到用户定义动作模式！");
}
