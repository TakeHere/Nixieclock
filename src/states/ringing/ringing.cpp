#include <Arduino.h>

#include <nixie.h>
#include <rotencoder.h>
#include "externalboxmanager.h"
#include "timemanager.h"
#include "consts.h"

unsigned long previousMillis = 0;
bool display_on = true;

unsigned long startingMillis = 0;

void init_ringing(){
    init_ringtone();
    reset_counter();

    startingMillis = millis();
}

void blinking_time_loop(){
    if (millis() - previousMillis >= ringingInterval) {
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
    blinking_time_loop();

    if(is_sw_pressed()){
        stop_ringtone();
        set_vibration(0);

        return true;
    }else{
        unsigned long timeSinceStart = millis() - startingMillis;
        
        int vibration = (int)((timeSinceStart / (float) timeUntilMaxVibrations) * 255);
        vibration = constrain(vibration, 0, 255);

        set_vibration(vibration);
        loop_ringtone();
        
        return false;
    }
}