#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>

#include <consts.h>
#include "secrets.h"
#include <utils.h>
#include <alarmmanager.h>
#include "nixie.h"

/*
IPAddress local_IP(192, 168, 1, 216);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
*/

AsyncWebServer server(80);

const char* PARAM_REPEATABLE = "repeatable";
const char* PARAM_DAYS = "days";
const char* PARAM_TIME = "time";


void setupWifi(){
  // Connect to wifi
  WiFi.mode(WIFI_STA);

    /*
    if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
    */

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED);

  String ip = WiFi.localIP().toString();
  int lastDotIndex = ip.lastIndexOf('.');
  if (lastDotIndex != -1 && lastDotIndex < ip.length() - 1) {
    displayNumber(ip.substring(lastDotIndex + 1));
  }

  delay(1000);
}

void setupServer(){
     // Configurez CORS
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    server.on("/addAlarm", HTTP_POST, [] (AsyncWebServerRequest *request) {
        String repeatable = "";
        String days = "";
        String time = "";

        if (request->hasParam(PARAM_REPEATABLE, true)) {
            repeatable = request->getParam(PARAM_REPEATABLE, true)->value();
        }
        if (request->hasParam(PARAM_DAYS, true)) {
            days = request->getParam(PARAM_DAYS, true)->value();
        }
        if (request->hasParam(PARAM_TIME, true)) {
            time = request->getParam(PARAM_TIME, true)->value();
        }

        if (repeatable != "" && days != "" && time != "") {
            bool isRepeatable = (repeatable == "true") ? true : false;
            
            int hours, minutes;
            convertTimeStringToInts(time, hours, minutes); 

            bool daysArray[7];
            int arraySize;
            parseDaysString(days, daysArray, arraySize);

            add_alarm(hours, minutes, daysArray, isRepeatable);

            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Missing parameters / Bad parameters");
        }
    });

    server.on("/removeAlarm", HTTP_POST, [] (AsyncWebServerRequest *request) {
        String repeatable = "";
        String days = "";
        String time = "";

        if (request->hasParam(PARAM_REPEATABLE, true)) {
            repeatable = request->getParam(PARAM_REPEATABLE, true)->value();
        }
        if (request->hasParam(PARAM_DAYS, true)) {
            days = request->getParam(PARAM_DAYS, true)->value();
        }
        if (request->hasParam(PARAM_TIME, true)) {
            time = request->getParam(PARAM_TIME, true)->value();
        }

        if (repeatable != "" && days != "" && time != "") {
            bool isRepeatable = (repeatable == "true") ? true : false;
            
            int hours, minutes;
            convertTimeStringToInts(time, hours, minutes); 

            bool daysArray[7];
            int arraySize;
            parseDaysString(days, daysArray, arraySize);

            remove_alarm(hours, minutes, daysArray, isRepeatable);

            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Missing parameters / Bad parameters");
        }
    });

    server.on("/getAlarms", HTTP_GET, [](AsyncWebServerRequest *request){
        String jsonAlarms = generateAlarmsJSON();

        request->send(200, "application/json", jsonAlarms);
    });

    server.begin();
}