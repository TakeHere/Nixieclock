#include <Arduino.h>

#include "TonePlayer.h"

const PROGMEM int8_t SONG[] = {
  NB3, 0,
  REST, -2,
  NFIS4, 0,
  REST, -2,
  NE4, 0,
  REST, -2,
  NB3, 0,
  REST, -2,
  NDIS4, 0,
  REST, -1,
  REST, -7,
  NDIS4, 0,
  REST, -1,
  NE4, 0,
  REST, -2,
  REST, -3,
  NB3, 0,
  REST, -2,
  NE4, 0,
  REST, -2,
  NB3, 0,
  REST, -2,
  NDIS4, 0,
  REST, -1,
  REST, -7,
  NDIS4, 0,
  REST, -1,
  NE4, 0,
  REST, -2,
  REST, -3,
  NB3, 0,
  REST, -2,
  NFIS4, 0,
  REST, -2,
  NE4, 0,
  REST, -2,
  NB3, 0,
  REST, -2,
  NDIS4, 0,
  REST, -1,
  REST, -7,
  NDIS4, 0,
  REST, -1,
  NE4, 0,
  REST, -2,
  REST, -3,
  NB3, 0,
  REST, -2,
  NE4, 0,
  REST, -2,
  NGIS4, 0,
  REST, -2,
  NFIS4, 0,
  REST, -1,
  NE4, 0,
  REST, -1,
  NFIS4, 0,
  REST, -2
};


#define VIBRATION_PIN 22
#define BUZZER_PIN 23


TonePlayer top(BUZZER_PIN);

void setup_externalbox(){
    pinMode(VIBRATION_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void set_vibration(bool vibrate){
    digitalWrite(VIBRATION_PIN, vibrate);
}

void init_ringtone(){
    top.setSong(SONG, sizeof(SONG), 80);
    top.play(); 
}

void loop_ringtone(){
    top.loop();
    top.setOnEndOfSongCallback(init_ringtone);
}

void stop_ringtone(){
    top.noTone();
}