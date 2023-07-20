#include <setup.h>

void setup() {
    Serial.begin(115200);
    io_setup();
    wifi_setup();
    timeClient.begin();
    Serial.println(F("ESP Relay - setup complete"));
}

void loop() {
    wifi_loop();
    io_loop();
    time_obtain();
    loop_counter++;
}
