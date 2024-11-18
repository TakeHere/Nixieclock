#include <Arduino.h>
#include <nixie.h>
#include "timemanager.h"

static int interval = 1000 / 2 / 4;

static int currentDigit;              
static int direction;                   
static int cycleCount;                  
static unsigned long previousMillis;   
static String currentNumber;

void init_antipoisoning(){
    cycleCount = 0;
    direction = 1;
    currentDigit = 0;
    previousMillis = 0;
    currentNumber = "0000";
}

bool loop_antipoisoning() {
    unsigned long currentMillis = millis();

    if (cycleCount >= 3) {
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            String formattedTime = get_formatted_time();
                
            for (int i = 3; i >= 0; i--){
                int digitAtIndex = currentNumber.charAt(i) - '0';
                int timeAtIndex = formattedTime.charAt(i) - '0';

                if(digitAtIndex < timeAtIndex){
                    digitAtIndex += 1;
                    char modifiedDigitChar = digitAtIndex + '0';
                    currentNumber.setCharAt(i, modifiedDigitChar);

                    return false;
                }else if (digitAtIndex > timeAtIndex){
                    digitAtIndex -= 1;
                    char modifiedDigitChar = digitAtIndex + '0';
                    currentNumber.setCharAt(i, modifiedDigitChar);

                    return false;
                }
            }

            return true;
        }

        displayNumber(currentNumber);
    }else{
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            currentDigit += direction;

            if (currentDigit > 9) {
                currentDigit = 9;
                direction = -1; // Descending order direction
            } else if (currentDigit < 0) {
                currentDigit = 0;
                direction = 1; // Ascending order direction
                cycleCount++;
            }
        }

        displayRepeatingDigit(currentDigit);
    }

    
    return false;
}



//Another antipoisoning pattern

/*
#include <Arduino.h>
#include <nixie.h>

bool loop_antipoisoning(){
    for (int z = 0; z < 170; z++){
        for (int i = 0; i < 10; i++){
            displayRepeatingDigit(i);

            delay(6);
        }
    }

    return true;
}

*/