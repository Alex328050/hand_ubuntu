#include <stdint.h>
#include "widget.h"

uint8_t handData_index = 0;
uint16_t myhandDataOK[17] = {ID1MAX-250, ID2MAX-122, ID3MAX-868, ID4MAX-267, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-49), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)};
uint16_t myhandDataYEAH[17] = {ID1MIN+200, ID2MIN+200, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos-(0), wristStartPos-(0), Servo1Start+(49), Servo2Start+(-16), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)};
uint16_t myhandDataPOINTFINGER[4][17] =
{
    {ID1MAX-370, ID2MAX-100, ID3MAX-1040, ID4MAX-240, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-47), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-336, ID2MAX-111, ID3MAX-0, ID4MAX-0, ID5MAX-941, ID6MAX-310, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-67), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-140, ID2MAX-165, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-1210, ID8MAX-300, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-86), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-30, ID2MAX-100, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-910, ID10MAX-280, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-101), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
};
uint16_t myhandDataWIDEANDSHRINK[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start-(0), Servo2Start-(18), Servo3Start-(0), Servo4Start-(-10), Servo5Start-(-26)},
    {ID1MIN+200, ID2MIN+200, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataPRESS[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX-500, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX-800, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataSWING[10][17] =
{
    {ID1MIN+100, ID2MIN+450, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+100, ID2MIN+450, ID3MIN, ID4MIN, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+100, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+100, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+100, ID2MIN+450, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX,         ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataACTION33[10][17] =
{
    {ID1MAX-490, ID2MAX-354, ID3MAX-0, ID4MAX-0, ID5MAX-1151, ID6MAX-455, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-39), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-490, ID2MAX-354, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-39), Servo2Start-(15), Servo3Start-(0), Servo4Start-(0), Servo5Start-(0)},
    {ID1MAX-490, ID2MAX-354, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-39), Servo2Start-(15), Servo3Start-(0), Servo4Start-(-13), Servo5Start-(0)},
    {ID1MAX-490, ID2MAX-354, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-39), Servo2Start-(15), Servo3Start-(0), Servo4Start-(-13), Servo5Start-(-25)},
    {ID1MAX-0, ID2MAX-0, ID3MAX-0, ID4MAX-0, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(15), Servo3Start-(0), Servo4Start-(-13), Servo5Start-(-25)},
    {ID1MAX-0, ID2MAX-0, ID3MAX-1180, ID4MAX-348, ID5MAX-1151, ID6MAX-455, ID7MAX-1250, ID8MAX-390, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(-25)},
    {ID1MAX-155, ID2MAX-172, ID3MAX-763, ID4MAX-255, ID5MAX-804, ID6MAX-298, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-60), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(-25)},
    {ID1MAX-0, ID2MAX-0, ID3MAX-0, ID4MAX-0, ID5MAX-1151, ID6MAX-455, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(12), Servo3Start-(0), Servo4Start-(0), Servo5Start-(-25)},
    {ID1MAX-490, ID2MAX-279, ID3MAX-1180, ID4MAX-0, ID5MAX-1151, ID6MAX-455, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-40), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(-25)},
    {ID1MAX-490, ID2MAX-0, ID3MAX-1180, ID4MAX-348, ID5MAX-1151, ID6MAX-455, ID7MAX-1250, ID8MAX-390, ID9MAX-1120, ID10MAX-410, wristStartPos-(0), wristStartPos-(0), Servo1Start-(-40), Servo2Start-(0), Servo3Start-(0), Servo4Start-(0), Servo5Start-(-25)},
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
    {ID1MAX-304, ID2MAX-427, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
    {ID1MAX-304, ID2MAX-670, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
    {ID1MAX-304, ID2MAX-670, ID3MAX-316, ID4MAX-860, ID5MAX-0, ID6MAX-0, ID7MAX-0, ID8MAX-0, ID9MAX-0, ID10MAX-0, wristStartPos-(0), wristStartPos-(0), Servo1Start-(0), Servo2Start-(-1), Servo3Start-(-2), Servo4Start-(-1), Servo5Start-(0)},
};
