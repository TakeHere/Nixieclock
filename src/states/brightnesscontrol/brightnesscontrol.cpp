#include <Arduino.h>

#include <utils.h>
#include <nixie.h>
#include <rotencoder.h>
#include <timemanager.h>


unsigned long last_mouvementBrightness = 0;
const long inactivity_thresholdBrightness = 2000;

bool loop_brightnessControl(){
    // If no movement for n seconds, cancel adding alarm
    if (has_rotary_changed()){
        last_mouvementBrightness = millis();
    }

    if (millis() - last_mouvementBrightness >= inactivity_thresholdBrightness) {
        last_mouvementBrightness = millis();
        return true;
    }

    
    // changing brightness part
    int brightnessLevel = get_rotencoder_count();
    brightnessLevel = constrain(brightnessLevel, 0, 9);

    // clamp rotary encoder
    set_counter(brightnessLevel);

    displayRepeatingDigit(brightnessLevel);

    // Remap the brightnessLevel to the PWM interval
    set_brightness(map(brightnessLevel, 0, 9, 20, 1));

    if (is_sw_pressed()){
        set_rotencoder_acceleration(true);
        reset_counter();

        turn_display_off();
        delay(500);

        return true;
    }
    

    return false;
}

void init_brightnessControl(){
    reset_counter();
    last_mouvementBrightness = millis();
    set_rotencoder_acceleration(false);
}