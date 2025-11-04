#include <stdint.h>
#include "widget.h"

uint8_t handData_index = 0;
uint16_t myhandDataOK[17] = {ID1MAX-730, ID2MAX-148, ID3MAX-1035, ID4MAX-875, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos-(0), wristStartPos-(0), Servo1Start-(49), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)};
uint16_t myhandDataYEAH[17] = {ID1MIN+600, ID2MIN+600, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos-(0), wristStartPos-(0), Servo1Start-(49), Servo2Start-(-16), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)};
uint16_t myhandDataPOINTFINGER[4][17] =
{
    {ID1MAX-617, ID2MAX-257, ID3MAX-1112, ID4MAX-758, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(47), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-617, ID2MAX-332, ID3MAX-0, ID4MAX-0, ID5MAX-1322, ID6MAX-851, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(62), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-496, ID2MAX-332, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-985, ID8MAX-765, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(82), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-465, ID2MAX-332, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-859, ID10MAX-708, wristStartPos-(0), wristStartPos-(0), Servo1Start-(94), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},

};
uint16_t myhandDataWIDEANDSHRINK[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start-(0), Servo2Start-(-14), Servo3Start-(0), Servo4Start-(14), Servo5Start-(27)},
    {ID1MAX-462, ID2MAX-714, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataPRESS[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX-500, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX-800, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataSWING[10][17] =
{
    {ID1MIN+300, ID2MIN+450, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+450, ID3MIN, ID4MIN, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataACTION33[10][17] =
{
    {ID1MAX-1024, ID2MAX-505, ID3MAX, ID4MAX, ID5MAX-1490, ID6MAX-1130, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(29), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-1024, ID2MAX-505, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(29), Servo2Start-(-12), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-1024, ID2MAX-505, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(29), Servo2Start-(-12), Servo3Start-(0), Servo4Start-(9), Servo5Start-(0)},
    {ID1MAX-1024, ID2MAX-505, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos-(0), wristStartPos-(0), Servo1Start-(29), Servo2Start-(-12), Servo3Start-(0), Servo4Start-(9), Servo5Start-(18)},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(-12), Servo3Start-(0), Servo4Start-(9), Servo5Start-(18)},
    {ID1MAX, ID2MAX, ID3MAX-1260, ID4MAX-875, ID5MAX-1490, ID6MAX-1130, ID7MAX-1178, ID8MAX-1110, ID9MAX, ID10MAX, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(36)},
    {ID1MAX-746, ID2MAX-381, ID3MAX-1260, ID4MAX-581, ID5MAX-1490, ID6MAX-742, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(54), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX-1490, ID6MAX-1130, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(-6), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-1024, ID2MAX-476, ID3MAX-1260, ID4MAX, ID5MAX-1490, ID6MAX-1130, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(32), Servo2Start-(-6), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-1024, ID2MAX, ID3MAX-1260, ID4MAX-875, ID5MAX-1490, ID6MAX-1130, ID7MAX-1178, ID8MAX-1110, ID9MAX-1175, ID10MAX-987, wristStartPos-(0), wristStartPos-(0), Servo1Start-(32), Servo2Start-(-6), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
};

uint16_t myhandDataPiano[8][17] =
{
    // {ID1MAX, ID2MAX, ID3MAX-450, ID4MAX-350, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+6, Servo3Start, Servo4Start, Servo5Start-13},
    // {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX-450, ID8MAX-350, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+6, Servo3Start, Servo4Start, Servo5Start-13},
    // {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX-450, ID10MAX-350, wristStartPos, wristStartPos, Servo1Start, Servo2Start+6, Servo3Start, Servo4Start, Servo5Start-13},
    // {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+6, Servo3Start, Servo4Start, Servo5Start-13},
    {ID1MAX, ID2MAX, ID3MAX-450, ID4MAX-350, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+9, Servo3Start, Servo4Start, Servo5Start-8},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX-450, ID6MAX-350, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+9, Servo3Start, Servo4Start, Servo5Start-8},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX-450, ID10MAX-350, wristStartPos, wristStartPos, Servo1Start, Servo2Start+9, Servo3Start, Servo4Start, Servo5Start-8},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+9, Servo3Start, Servo4Start, Servo5Start-8},
};

uint16_t myhandDataScrew[3][17] =
{
    {ID1MAX-304, ID2MAX-427, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(49), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
    {ID1MAX-304, ID2MAX-670, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(49), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
    {ID1MAX-304, ID2MAX-670, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(41), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
};
