#ifndef NTP_TIME_CPP
#define NTP_TIME_CPP

#include <setup.h>

void time_obtain() {
    timeClient.update();
    if (loop_counter % 600000 == 40000) {
        time_day = timeClient.getDay();
        time_hour = timeClient.getHours();
        time_min = timeClient.getMinutes();
        time_unix = timeClient.getEpochTime();
        // Serial.println(time_unix);
    }
    time_scheduler();
    return;
}

void time_scheduler() {
    if (loop_counter % 100000 == 60000) {
        byte turn_off_relay_now = 0;
        if ((time_hour < 8 || time_hour > 22) && time_unix != 0) { // before morning, or after 10 pm
            turn_off_relay_now = 1;
        } else if (time_day == 0 && time_unix != 0) { // Sunday
            turn_off_relay_now = 2;
        }
        if (turn_off_relay_now != 0 && relay_status) { // turn off if turned on
            relay_status = false;
            relay_command = false;
#if TOGGLE_ONBOARD_LED_FOR_TESTING == 1
            digitalWrite(station_status_led, SS_LED_NORMALY_LOW);
#endif
            digitalWrite(relay_pin, LOW);
            Serial.print(F("Schedule offtime relay turned off: "));
            if (turn_off_relay_now == 1) {
                Serial.print(F("of hours"));
            } else if (turn_off_relay_now == 2) {
                Serial.print(F("it's Sunday"));
            } else {
                Serial.print(F("no reason at all"));
            }
            Serial.println();
        }
    }
    return;
}

#endif