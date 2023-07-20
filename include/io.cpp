#ifndef IO_CPP
#define IO_CPP

#include <setup.h>

ICACHE_RAM_ATTR void soldering_station_alive_interrupt() {
    timer_last_SSalive = millis();
    return;
}

void io_setup() {
    pinMode(soldering_station_alive_pin, INPUT);
    pinMode(station_status_led, OUTPUT);
    pinMode(iron_status_led, OUTPUT);
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW);
    digitalWrite(station_status_led, SS_LED_NORMALY_LOW);
    digitalWrite(iron_status_led, IRON_LED_NORMALY_LOW);
    pinMode(soldering_iron_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(soldering_station_alive_pin), soldering_station_alive_interrupt, RISING);
    return;
}

void io_loop() {
    soldering_station_status_function();
    soldering_iron_status_function();
    relay_function();
    printStatus(0);
    return;
}

void soldering_station_status_function() {
    bool previous_soldering_station_status = soldering_station_status;
    if (millis() < (timer_last_SSalive + SSalive_max_timeout)) {
        soldering_station_status = true;
    } else {
        soldering_station_status = false;
    }
    if (!previous_soldering_station_status && soldering_station_status) { // soldering station turned on
        timer_since_last_SS_turn_on = millis() + 1000;
        Serial.println(F("Soldering station turned on"));
        overwrite_relay_to_off = false;
        digitalWrite(station_status_led, SS_LED_NORMALY_HIGH);
    } else if (previous_soldering_station_status && !soldering_station_status) { // soldering station turned off
        timer_since_last_SS_turn_on = 0;
        Serial.println(F("Soldering station turned off"));
        digitalWrite(station_status_led, SS_LED_NORMALY_LOW);
        relay_allowed_to_be_on = false;
        if (relay_status) {
            overwrite_relay_to_off = true;
            timer_turn_off_override = millis() + relay_turn_off_relay_delay_ms;
        }
    }
    if (soldering_station_status && millis() > (timer_since_last_SS_turn_on) && timer_since_last_SS_turn_on > 0 && !relay_allowed_to_be_on) {
        relay_allowed_to_be_on = true;
        Serial.println(F("Relay can now be turned on if needed"));
    }
    return;
}

void soldering_iron_status_function() {
    bool previous_soldering_iron_pin_status = soldering_iron_pin_status;
    soldering_iron_pin_status = digitalRead(soldering_iron_pin);
    if (!soldering_station_status && soldering_iron_pin_status != previous_soldering_iron_pin_status) {
        timer_last_soldering_iron_state_change = millis() + relay_turn_on_relay_delay_ms;
    } else if (soldering_station_status && soldering_iron_pin_status && !previous_soldering_iron_pin_status) {
        soldering_iron_status_stage1 = true;
        timer_last_soldering_iron_state_change = millis() + relay_turn_on_relay_delay_ms;
    } else if (soldering_station_status && !soldering_iron_pin_status && previous_soldering_iron_pin_status) {
        soldering_iron_status_stage1 = false;
        timer_last_soldering_iron_state_change = millis() + relay_turn_on_relay_delay_ms;
    } else if (soldering_station_status && soldering_iron_pin_status && previous_soldering_iron_pin_status && soldering_iron_status_stage1 && millis() > timer_last_soldering_iron_state_change && !soldering_iron_status_stage2) {
        Serial.println(F("Soldering iron is out of the dock"));
        soldering_iron_status_stage2 = true;
        digitalWrite(iron_status_led, IRON_LED_NORMALY_HIGH);
    } else if (soldering_station_status && !soldering_iron_pin_status && !previous_soldering_iron_pin_status && !soldering_iron_status_stage1 && millis() > timer_last_soldering_iron_state_change && soldering_iron_status_stage2) {
        Serial.println(F("Soldering iron is in the dock"));
        soldering_iron_status_stage2 = false;
        digitalWrite(iron_status_led, IRON_LED_NORMALY_LOW);
    } else if (!soldering_station_status && !soldering_iron_pin_status && !previous_soldering_iron_pin_status && millis() > timer_last_soldering_iron_state_change) {
        Serial.println(F("Soldering iron is in the dock, soldering station is off"));
        digitalWrite(iron_status_led, IRON_LED_NORMALY_LOW);
    } else if (!soldering_station_status && soldering_iron_pin_status && previous_soldering_iron_pin_status && millis() > timer_last_soldering_iron_state_change) {
        Serial.println(F("Soldering iron is out the dock, soldering station is off"));
        digitalWrite(iron_status_led, IRON_LED_NORMALY_HIGH);
    }
    return;
}

void relay_function() {
    if (!soldering_iron_status_stage2 && relay_allowed_to_be_on && !relay_status) { // turn on relay
        relay_command = true;
        timer_last_relay_change = millis();
    } else if (soldering_iron_status_stage2 && relay_allowed_to_be_on && relay_status) { // turn off relay
        relay_command = false;
        timer_last_relay_change = millis();
    } else if (!relay_allowed_to_be_on && relay_status) {
        relay_command = false;
        timer_last_relay_change = millis();
    }

    if (!relay_command && relay_status && millis() > (timer_last_relay_change + relay_turn_off_relay_delay_ms)) {
        relay_status = false;
        digitalWrite(relay_pin, LOW);
        Serial.println(F("Relay turned off"));
    } else if (relay_command && !relay_status && millis() > (timer_last_relay_change + relay_turn_on_relay_delay_ms) && relay_allowed_to_be_on) {
        relay_status = true;
        digitalWrite(relay_pin, HIGH);
        Serial.println(F("Relay turned on"));
    } else if (relay_status && overwrite_relay_to_off && millis() > timer_turn_off_override) {
        relay_status = false;
        digitalWrite(relay_pin, LOW);
        Serial.println(F("Relay turned off, override"));
    }
    return;
}

void printStatus(bool enable) {
    if (enable) {
        if (loop_counter % 100000 == 0) {
            Serial.print(F("Station status: "));
            Serial.print(soldering_station_status);
            Serial.print(F(", iron status: "));
            Serial.println(digitalRead(soldering_iron_pin));
        }
    }
    return;
}

#endif