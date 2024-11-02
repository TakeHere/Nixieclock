#include <Arduino.h>
#include <consts.h>
#include <utils.h>
#include "rotencoder.h"

int brightness = 1;

void setupTubes(){
    for (int iDisplay = 0; iDisplay < 4; iDisplay++) {
        for (int iPin = 0; iPin < 4; iPin++) {
            pinMode(displayPins[iDisplay][iPin], OUTPUT);
        }
    }
}

void displayDigit(int tube, int digit){
    digit = (digit >= 0 && digit <= 9) ? digit : 0;

    // 1 - 9
    int val = brightness;
    if(val == 0) val = 1;

    if(millis() % val == 0){
      digitalWrite(displayPins[tube][0], digits[digit][0]);
      digitalWrite(displayPins[tube][1], digits[digit][1]);
      digitalWrite(displayPins[tube][2], digits[digit][2]);
      digitalWrite(displayPins[tube][3], digits[digit][3]);
    }else{
      digitalWrite(displayPins[tube][0], HIGH);
      digitalWrite(displayPins[tube][1], HIGH);
      digitalWrite(displayPins[tube][2], HIGH);
      digitalWrite(displayPins[tube][3], HIGH);
    }
}

void displayNumber(int number){
  // Clamp number for safety
  if (number < 0) {
      number = 0;
  } else if (number > 9999) {
      number = 9999;
  }

  // Add zeros where it is needed
  int len = countDigits(number);
  int zerosToAdd = 4 - len;
  String numStr = String(number);
  for (int i = 0; i < zerosToAdd; i++){
      numStr = "0" + numStr;
  }

  // Set the digits onto the tubes
  for (int i = 0; i < 4; i++){
    displayDigit(i, numStr.charAt(i) - '0');
  }
}

void displayNumber(String number){
  // Set the digits onto the tubes
  for (int i = 0; i < 4; i++){
    displayDigit(i, number.charAt(i) - '0');
  }
}

void displayNumber(int hours, int minutes){
  String numberStr = formatTime(hours) + formatTime(minutes);
  displayNumber(numberStr);
}

void displayRepeatingDigit(int digit){
  displayNumber(String(digit) + String(digit) + String(digit) + String(digit));
}

void turn_display_off(){
  for (int iDisplay = 0; iDisplay < 4; iDisplay++) {
        for (int iPin = 0; iPin < 4; iPin++) {
            digitalWrite(displayPins[iDisplay][iPin], HIGH);
        }
    }
}

void set_brightness(int brightnessValue){
  brightness = brightnessValue;
}

int get_brightness(){
  return brightness;
}