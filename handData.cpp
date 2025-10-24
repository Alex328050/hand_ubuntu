#include <stdint.h>
#include "widget.h"

uint8_t handData_index = 0;
uint16_t myhandDataOK[17] = {ID1MAX-100, ID2MIN+150, ID3MAX-100, ID4MIN+150, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start};
uint16_t myhandDataYEAH[17] = {ID1MAX-400, ID2MIN+100, ID3MAX-100, ID4MIN, ID5MAX-200, ID6MIN+50, ID7MAX-400, ID8MIN, ID9MAX-400, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start};
uint16_t myhandDataPOINTFINGER[4][17] =
{
    {1224, 1110, 1110, 1282, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, 47, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {1224, 1110, ID3MAX, ID4MAX, 1029, 980, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, 33, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {1224, 1110, ID3MAX, ID4MAX, ID5MAX, ID6MAX, 1037, 980, ID9MAX, ID10MAX, wristStartPos, wristStartPos, 15, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {1224, 1110, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, 531, 1176, wristStartPos, wristStartPos, 0, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataWIDEANDSHRINK[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, 120, 75, 49, 19},
    {ID1MAX, 890, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, 59, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataPRESS[2][17] =
{
    {ID1MAX, ID2MAX, ID3MAX-500, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX-800, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataSWING[10][17] =
{
    {ID1MIN+300, ID2MIN+250, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+250, ID3MIN, ID4MIN, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+250, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+250, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MIN+300, ID2MIN+250, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MIN, ID4MIN, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MIN, ID6MIN, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MIN, ID8MIN, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MIN, ID10MIN, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start, Servo3Start, Servo4Start, Servo5Start},
};
uint16_t myhandDataACTION33[33][17] =
{
    {1275, 2000, 706, 1221, 539, 871, 514, 1355, 155, 1158, 800, 800, 110, 81, 66, 65, 48},
    {1147, 903, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 110, 81, 66, 65, 48},
    {976, 1221, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 82, 81, 66, 65, 48},
    {1415, 1303, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 102, 81, 66, 65, 48},
    {1236, 979, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 89, 81, 66, 65, 48},
    {988, 765, 1382, 1375, 789, 1104, 945, 1320, 486, 1102, 800, 800, 98, 81, 66, 65, 48},
    {988, 765, 1382, 1375, 789, 1282, 981, 1515, 614, 706, 800, 800, 98, 81, 66, 65, 48},
    {1255, 648, 1382, 1375, 867, 1266, 514, 939, 155, 706, 800, 800, 74, 81, 66, 65, 48},
    {1135, 1290, 1213, 936, 718, 1249, 757, 1658, 607, 706, 800, 800, 80, 105, 66, 60, 7},
    {1135, 1290, 1213, 936, 718, 1026, 757, 1502, 554, 1041, 800, 800, 80, 105, 66, 60, 36},
    {1135, 1290, 1213, 1380, 718, 1293, 757, 1658, 554, 1315, 800, 800, 35, 105, 66, 60, 36},
    {1026, 1234, 706, 1380, 539, 1293, 514, 1658, 155, 1315, 800, 800, 28, 90, 66, 60, 0},
    {925, 1234, 1160, 1380, 539, 1427, 514, 1476, 155, 980, 800, 800, 48, 90, 66, 58, 30},
    {2000, 2000, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 99, 90, 66, 58, 30},
    {956, 986, 706, 857, 539, 637, 514, 939, 155, 706, 800, 800, 88, 80, 66, 58, 30},
    {956, 986, 1762, 2000, 1231, 2000, 514, 939, 155, 706, 800, 800, 76, 88, 62, 58, 30},
    {956, 986, 1366, 2000, 986, 2000, 1151, 2000, 870, 2000, 800, 800, 31, 88, 62, 58, 30},
    {1313, 986, 1366, 2000, 986, 2000, 896, 1606, 155, 706, 800, 800, 66, 93, 64, 58, 30},
    {1255, 820, 1202, 1449, 587, 1104, 514, 939, 155, 706, 800, 800, 66, 93, 64, 58, 30},
    {791, 986, 822, 1109, 587, 893, 514, 1160, 155, 706, 800, 800, 95, 85, 54, 58, 30},
    {823, 1214, 1657, 2000, 1064, 2000, 1175, 2000, 893, 2000, 800, 800, 25, 76, 57, 58, 30},
    {1166, 1214, 706, 2000, 539, 1466, 738, 1134, 155, 706, 800, 800, 100, 78, 66, 54, 30},
    {1268, 1110, 1340, 1076, 1255, 2000, 1230, 2000, 577, 1330, 800, 800, 65, 81, 66, 61, 38},
    {1205, 1020, 1002, 1076, 539, 637, 514, 939, 155, 706, 800, 800, 98, 81, 66, 61, 38},
    {1065, 1227, 706, 1482, 539, 1115, 514, 1229, 155, 706, 800, 800, 75, 81, 66, 65, 32},
    {766, 1448, 880, 1370, 539, 1438, 538, 1541, 155, 706, 800, 800, 83, 81, 66, 65, 32},
    {766, 1448, 880, 1370, 539, 954, 538, 939, 155, 706, 800, 800, 83, 81, 66, 65, 32},
    {1071, 655, 1403, 2000, 539, 926, 538, 1104, 155, 706, 800, 800, 90, 81, 66, 65, 32},
    {1122, 2000, 706, 857, 539, 637, 538, 939, 155, 706, 800, 800, 96, 81, 66, 65, 32},
    {1122, 2000, 1393, 1286, 2000, 2000, 2000, 2000, 2000, 1472, 800, 800, 68, 81, 66, 60, 32},
    {1122, 793, 1456, 2000, 539, 1132, 514, 1320, 155, 742, 800, 800, 107, 81, 66, 60, 32},
    {740, 1138, 706, 1244, 2000, 2000, 2000, 2000, 1059, 1396, 800, 800, 107, 81, 66, 60, 32},
    {1255, 1076, 1213, 1193, 867, 954, 514, 939, 155, 706, 800, 800, 52, 81, 66, 65, 48},
};

uint16_t myhandDataPiano[4][17] =
{
    {ID1MAX, ID2MAX, ID3MAX-450, ID4MAX-350, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+15, Servo3Start, Servo4Start, Servo5Start-15},
    {ID1MAX, ID2MAX, ID3MAX-450, ID4MAX-350, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+15, Servo3Start, Servo4Start, Servo5Start-15},
    {ID1MAX, ID2MAX, ID3MAX-450, ID4MAX-350, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+15, Servo3Start, Servo4Start, Servo5Start-15},
    {ID1MAX, ID2MAX, ID3MAX, ID4MAX, ID5MAX, ID6MAX, ID7MAX, ID8MAX, ID9MAX, ID10MAX, wristStartPos, wristStartPos, Servo1Start, Servo2Start+15, Servo3Start, Servo4Start, Servo5Start-15},
};
