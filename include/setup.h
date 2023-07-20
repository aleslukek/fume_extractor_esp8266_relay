#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <config.h>
//#include <passwords.h> // this redefines passwords in config with #undef and #define
#include <AutoConnect.h>
#include <NTPClient.h>

#define soldering_station_alive_pin 14 // Gray cable
#define soldering_iron_pin 12          // White cable
#define relay_pin 5                    // relay pin
#define station_status_led 2           // soldering station status
#define iron_status_led 16             // soldering iron status

#define SS_LED_NORMALY_HIGH LOW
#define SS_LED_NORMALY_LOW HIGH
#define IRON_LED_NORMALY_HIGH LOW
#define IRON_LED_NORMALY_LOW HIGH

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

int relay_turn_off_relay_delay_ms = relay_turn_off_relay_delay_ms_def;
bool soldering_station_status = false;
unsigned long timer_last_SSalive = 0;
unsigned long timer_since_last_SS_turn_on = 0;
bool relay_allowed_to_be_on = false;  // this allows relay to turn off
bool soldering_iron_pin_status = LOW;
bool soldering_iron_status_stage1 = false;
bool soldering_iron_status_stage2 = false;
unsigned long timer_last_soldering_iron_state_change = 0;
unsigned long timer_last_relay_change = 0;
bool overwrite_relay_to_off = false;
unsigned long timer_turn_off_override = 0;
bool relay_status = false;
bool relay_command = false;
bool print_local_ip = false;
unsigned long loop_counter = 0;

byte time_day;
byte time_hour;
byte time_min;
const String days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long time_unix;
unsigned long time_last_relay_on;
//#define SHOW_TIME

//WebServer server;
ESP8266WebServer server(80);
AutoConnect portal(server);
AutoConnectConfig config;

void wifi_setup();
void wifi_loop();
void exitOTAStart();
void exitOTAProgress(unsigned int amount, unsigned int sz);
void exitOTAEnd();
void exitOTAError(uint8_t err);
bool atDetect(IPAddress& softapIP);
void handleGPIO();
void handleRoot();
void sendRedirect(String uri);
void io_setup();
void io_loop();
ICACHE_RAM_ATTR void solering_station_alive_interrupt();
void soldering_station_status_function();
void soldering_iron_status_function();
void relay_function();
void printStatus(bool enable);
void time_obtain();
void time_scheduler();

#include <io.cpp>
#include <ntp.cpp>
#include <wifi.cpp>

#endif