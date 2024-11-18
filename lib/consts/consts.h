// consts.h
#ifndef CONSTS_H
#define CONSTS_H

#include <WiFi.h>


#define HV_SW_PINOUT 12

extern int displayPins[4][4];
extern int digits[10][4];

extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

extern const int sleepStartTime[2];
extern const int sleepEndTime[2];

// in ms
extern int antipoisoning_delay;


// Ringing
extern const int timeUntilMaxVibrations;
extern const int ringingInterval;

#endif // CONSTS_H