#ifndef MYINFERIOR_H
#define MYINFERIOR_H

#include <stdint.h>
#include <QSerialPort>
#include "widget.h"
#include "ui_widget.h"

extern uint16_t handData[17];
extern char transmitData[2+34+1+1+2];
extern QSerialPort* mySerialPort;

void calBit(uint16_t num, uint8_t *resultL, uint8_t *resultH);
uint8_t calCheckSum(char* data, uint8_t numOfData);
void calTransmit(uint16_t* handData, uint8_t isClearFault);

#endif // MYINFERIOR_H
