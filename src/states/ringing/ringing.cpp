#include <Arduino.h>

#include <nixie.h>
#include <rotencoder.h>
#include "externalboxmanager.h"
#include "timemanager.h"

unsigned long previousMillis = 0;
const long interval = 600;
bool display_on = true;

void init_ringing(){
    init_ringtone();
    reset_counter();
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
    blinking_time_loop();

    if(is_sw_pressed()){
        stop_ringtone();
        set_vibration(false);

        return true;
    }else{
        loop_ringtone();
        set_vibration(true);
        
        return false;
    }
}