#include <Arduino.h>

#include <nixie.h>
#include <rotencoder.h>
#include "externalboxmanager.h"
#include "timemanager.h"

unsigned long previousMillis = 0;
const long interval = 600;
bool display_on = true;

bool blinking_state = true;
int numberAddition1 = 0;
int numberAddition2 = 0;

void init_ringing(){
    blinking_state = true;
    reset_counter();
}

void addition_challenge(){
    init_ringtone();

    numberAddition1 = random(100);
    numberAddition2 = random(100);

    // Display addition numbers
    turn_display_off();
    delay(500);

    displayNumber(numberAddition1);
    delay(3000);

    displayNumber(numberAddition2);
    delay(3000);
}

void blinking_time_loop(){
    if (millis() - previousMillis >= interval) {
        previousMillis = millis();
        display_on = !display_on;
    }
    
    if (display_on){
        displayNumber(get_formatted_time());
    }else{
        turn_display_off(); 
    }
}

bool loop_ringing(){
    if (blinking_state){
        blinking_time_loop();
    
        if(is_sw_pressed()){
            blinking_state = false;

            addition_challenge();
        }
    }else{
        // Handle answer
        int rotencoder_count = get_rotencoder_count();

        displayNumber(rotencoder_count);
        if (is_sw_pressed()){
            if(rotencoder_count == numberAddition1 + numberAddition2){
                stop_ringtone();
                return true;
            }else {
                addition_challenge();
            }
        }
    }

    loop_ringtone();
    set_vibration(true);

    return false;
}