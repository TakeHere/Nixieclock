#include <Arduino.h>

#include <consts.h>

int countDigits(int number) {
  if (number == 0) return 1; // Cas particulier pour 0
  
  int count = 0;
  while (number != 0) {
    number /= 10;
    count++;
  }
  return count;
}

String formatTime(int timeNumber){
  if(timeNumber < 10){
    return "0" + String(timeNumber);
  }else{
    return String(timeNumber);
  }
}

void addOffsetToTime(int &heures, int &minutes, int offset) {
    minutes += offset;
    
    if (minutes >= 60) {
        heures += minutes / 60;
        minutes %= 60;
    } else if (minutes < 0) {
        heures += (minutes / 60) - 1;
        minutes = 60 + (minutes % 60);
    }

    if (heures >= 24) {
        heures %= 24;
    } else if (heures < 0) {
        heures = 24 + (heures % 24);
    }
}

void convertTimeStringToInts(const String& timeStr, int& hours, int& minutes) {
  int colonIndex = timeStr.indexOf(':');
  if (colonIndex != -1) {
    hours = timeStr.substring(0, colonIndex).toInt();
    minutes = timeStr.substring(colonIndex + 1).toInt();
  } else {
    hours = 0;
    minutes = 0;
  }
}

bool parseDaysString(const String& daysStr, bool daysArray[], int& arraySize) {
  arraySize = 0;
  int startIndex = 0;
  int commaIndex;

  while (startIndex < daysStr.length() && arraySize < 7) {
    commaIndex = daysStr.indexOf(',', startIndex);
    if (commaIndex == -1) {
      commaIndex = daysStr.length();
    }

    String dayStr = daysStr.substring(startIndex, commaIndex);
    dayStr.trim();

    if (dayStr == "true") {
      daysArray[arraySize] = true;
    } else if (dayStr == "false") {
      daysArray[arraySize] = false;
    } else {
      return false;
    }

    arraySize++;
    startIndex = commaIndex + 1;
  }

  return true;
}

void set_hv_sw(bool state){
  digitalWrite(HV_SW_PINOUT, !state);
}

void sleep_for(int seconds){
  set_hv_sw(false);

  esp_sleep_enable_timer_wakeup(seconds * 1000000);
  esp_light_sleep_start();  

  set_hv_sw(true);  
}