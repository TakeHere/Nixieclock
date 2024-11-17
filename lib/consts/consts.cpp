// consts.cpp
#include "consts.h"
#include <WiFi.h>

int displayPins[4][4] = {
  {16, 2, 15, 4},
  {19, 5, 17, 18},
  {33, 1, 21, 32},
  {13, 27, 25, 14}
};

int digits[10][4] =  {
  {0, 0, 0, 0}, 
  {1, 0, 0, 0}, 
  {0, 1, 0, 0},
  {1, 1, 0, 0}, 
  {0, 0, 1, 0}, 
  {1, 0, 1, 0},
  {0, 1, 1, 0}, 
  {1, 1, 1, 0}, 
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

const int sleepStartTime[2] = {9, 30};
const int sleepEndTime[2] = {17, 0};

const char* ntpServer = "pool.ntp.org";

// Adding 10s, time for the antipoisoning to end
int antipoisoning_delay = 300000 + 10000;