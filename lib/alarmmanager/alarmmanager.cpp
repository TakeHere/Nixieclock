#include <Arduino.h>

#include <timemanager.h>
#include <ArduinoJson.h>

struct Alarm {
    int minute;
    int hour;
    bool days[7];
    bool isRepeatable;
    int last_ringed_day;
};

const int MAX_EVENTS = 20;
Alarm alarms[MAX_EVENTS];
int alarm_count = 0;

void add_alarm(int hour, int minute, bool days[], bool isRepeatable) {
    alarms[alarm_count].minute = minute;
    alarms[alarm_count].hour = hour;
    for (int i = 0; i < 7; i++) {
        alarms[alarm_count].days[i] = days[i];
    }
    alarms[alarm_count].isRepeatable = isRepeatable;
    alarms[alarm_count].last_ringed_day = 100;
    alarm_count++;
}

void add_alarm_alldays(int hour, int minute, bool isRepeatable){
    bool days[7] = {true, true, true, true, true, true, true};
    add_alarm(hour, minute, days, isRepeatable);
}


void remove_alarm_index(int index) {
     if (index < 0 || index >= alarm_count) return;
    for (int i = index; i < alarm_count - 1; i++) {
        alarms[i] = alarms[i + 1];
    }
    alarm_count--;
}

void remove_alarm(int hour, int minute, bool days[], bool isRepeatable) {
    for (int i = 0; i < alarm_count; i++) {
        if (alarms[i].hour == hour && alarms[i].minute == minute && alarms[i].isRepeatable == isRepeatable) {
            bool days_match = true;
            for (int j = 0; j < 7; j++) {
                if (alarms[i].days[j] != days[j]) {
                    days_match = false;
                    break;
                }
            }
            if (days_match) {
                remove_alarm_index(i);
                i--;
            }
        }
    }
}

bool check_alarms(){
    int day = get_day();

    int time[2];
    get_time(time);

    int hour = time[0];
    int minute = time[1];

    for (int i = 0; i < alarm_count; i++){
        Alarm& alarm = alarms[i];
        
        if(alarm.hour == hour && alarm.minute == minute){
            if(alarm.last_ringed_day != day){
                if(alarm.days[day] == true){
                    alarm.last_ringed_day = day;

                    if(!alarm.isRepeatable) remove_alarm_index(i);

                    return true;
                }
            }
        }
    }
    return false;
}

String generateAlarmsJSON() {
    JsonDocument doc; // Ajustez la taille selon vos besoins
    JsonArray alarmsArray = doc.createNestedArray("alarms");

    for (int i = 0; i < alarm_count; i++) {
        JsonObject alarmObj = alarmsArray.createNestedObject();
        alarmObj["hour"] = alarms[i].hour;
        alarmObj["minute"] = alarms[i].minute;
        
        JsonArray daysArray = alarmObj.createNestedArray("days");
        for (int j = 0; j < 7; j++) {
            daysArray.add(alarms[i].days[j]);
        }
        
        alarmObj["isRepeatable"] = alarms[i].isRepeatable;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}