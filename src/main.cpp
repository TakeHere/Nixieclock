#include <Arduino.h>
#include <ArduinoOTA.h>

#include <consts.h>
#include <utils.h>
#include <main.h>

#include "nixie.h"
#include "rotencoder.h"
#include "timemanager.h"
#include "alarmmanager.h"
#include "alarmmanager.h"
#include "externalboxmanager.h"
#include "servermanager.h"


#include "states/antipoisoning/antipoisoning.h"
#include "states/ringing/ringing.h"
#include "states/addingalarm/addingalarm.h"
#include "states/brightnesscontrol/brigthnesscontrol.h"



enum StateMachine {
  AntiPoisoning,
  Ringing,
  AddingAlarm,
  DisplayingTime,
  BrightnessControl
};

enum StateMachine clock_state = DisplayingTime;

int lasttime_antipoisoning = 0;

void setup() {
  Serial.begin(115200);
  
  setupTubes();
  setupTime();

  setupRotencoder();
  setup_externalbox();

  setupWifi();
  setupServer();
  initOTA();

  // Enable HV transformer
  pinMode(HV_SW_PINOUT, OUTPUT);  
  set_hv_sw(true);

  pinMode(12, OUTPUT);  
  digitalWrite(12, LOW);
}

void loop() {
  read_sw();
  read_encoder();
  read_time();

  statemachine_loop();

  ArduinoOTA.handle();

  sleepcheck_loop();
}

void statemachine_loop(){
  switch (clock_state) {
    case AntiPoisoning:
      if(loop_antipoisoning()) clock_state = DisplayingTime;
      break;
    case Ringing:
      if(loop_ringing()) clock_state = DisplayingTime;
      break;
    case AddingAlarm:
      if(loop_addingalarm()) clock_state = DisplayingTime;
      break;
    case BrightnessControl:
      if(loop_brightnessControl()) clock_state = DisplayingTime;
      break;


    case DisplayingTime:
      // Transition into AntiPoisoning state
      if((millis() - lasttime_antipoisoning) > antipoisoning_delay){
        init_antipoisoning();
        lasttime_antipoisoning = millis();
        clock_state = AntiPoisoning;
        break;
      }

      // Transition into Alarm Ringing state
      if (check_alarms()){
        init_ringtone();
        init_ringing();
        clock_state = Ringing;
        break;
      }

      // Transition into Adding alarm state
      if (has_rotary_changed()){
        init_addingalarm();        
        clock_state = AddingAlarm;
        break;
      }

      // Transition into Brightness Control state
      if (is_sw_pressed()){
        init_brightnessControl();        
        clock_state = BrightnessControl;
        break;
      }

      displayNumber(get_formatted_time());
      break;
  }
}

void sleepcheck_loop(){
  int time[2];
  get_time(time);

  int hour = time[0];
  int minute = time[1];

  if(hour == sleepStartTime[0] && minute == sleepStartTime[1]){
    int sleepTimeHour = sleepEndTime[0] - sleepStartTime[0];
    int sleepTimeMinute = sleepEndTime[1] - sleepStartTime[1];

    int totalSleepSeconds = sleepTimeHour*3600 + sleepTimeMinute*60;

    sleep_for(totalSleepSeconds);
  }
}

void initOTA(){
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}