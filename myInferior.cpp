#include "myInferior.h"
#include "widget.h"

#define PI 3.14159265358979

uint16_t handData[17] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start};
char transmitData[2+34+1+1+2] = {0};
QSerialPort* mySerialPort;

void calBit(uint16_t num, uint8_t *resultL, uint8_t *resultH)
{
    uint8_t *pt = (uint8_t*)&num;
    *resultH = pt[1];
    *resultL = pt[0];
}

uint8_t calCheckSum(char* data, uint8_t numOfData)
{
    uint16_t checkSum = 0;
    uint8_t checkSumL, checkSumH;
    for (uint8_t i = 2; i < numOfData-1; ++i)
    {
        checkSum += data[i];
    }
    calBit(checkSum, &checkSumL, &checkSumH);
    return checkSumL;
}

void calTransmit(uint16_t* handData, uint8_t isClearFault)
{
    transmitData[0] = 0xFF;
    transmitData[1] = 0xFE;
    transmitData[2] = isClearFault;
    for (uint8_t i = 0; i < 17; ++i)
    {
        uint8_t dataL, dataH = 0;
        calBit(handData[i], &dataL, &dataH);
        transmitData[3+i*2] = dataL;
        transmitData[4+i*2] = dataH;
    }
    transmitData[2+34+1] = calCheckSum(transmitData, 2+34+1+1);
    transmitData[2+34+1+1] = 0xFD;
    transmitData[2+34+1+2] = 0xFC;
}
