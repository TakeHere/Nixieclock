#include <consts.h>

#include "time.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "utils.h"

int hours = 0;
int minutes = 0;
int day = 0;

void setupTime(){
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", ntpServer);
}

void read_time(){
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  day = timeinfo.tm_wday;
  day -= 1;

  if(day == -1) day = 6;
}

void get_time(int* timeArray){
  timeArray[0] = hours;
  timeArray[1] = minutes;
}

int get_day(){
  return day;
}

String get_formatted_time(){
  String hourStr = formatTime(hours);
  String minStr = formatTime(minutes);

  return (hourStr + minStr);
}

int get_formatted_time_int(){
  return get_formatted_time().toInt();
}