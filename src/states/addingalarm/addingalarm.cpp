#include <Arduino.h>

#include <utils.h>
#include <nixie.h>
#include <rotencoder.h>
#include <alarmmanager.h>
#include <timemanager.h>


unsigned long last_mouvement = 0;
const long inactivity_threshold = 3000;

int startingTime[2];

bool loop_addingalarm(){
    // If no movement for n seconds, cancel adding alarm
    if (has_rotary_changed()){
        last_mouvement = millis();
    }

    if (millis() - last_mouvement >= inactivity_threshold) {
        last_mouvement = millis();
        return true;
    }
    
    // Adding alarm part
    int rotencoder_count = get_rotencoder_count();
    int hours = startingTime[0];
    int minutes = startingTime[1];

    addOffsetToTime(hours, minutes, rotencoder_count);


    displayNumber(hours, minutes);
    if (is_sw_pressed()){
        add_alarm_alldays(hours, minutes, false);

        turn_display_off();
        delay(500);

        // Calculate and display the time difference between startingTime and hours:minutes
        int diffHours = hours - startingTime[0];
        int diffMinutes = minutes - startingTime[1];

        // Adjust for negative differences
        if (diffMinutes < 0) {
            diffHours--;
            diffMinutes += 60;
        }
        if (diffHours < 0) {
            diffHours += 24;
        }

        // Display the time difference
        displayNumber(diffHours, diffMinutes);
        delay(2000);  // Display for 2 seconds

        turn_display_off();
        delay(500);

        return true;
    }
    

    return false;
}

void init_addingalarm(){
    reset_counter();
    get_time(startingTime);
    last_mouvement = millis();
}