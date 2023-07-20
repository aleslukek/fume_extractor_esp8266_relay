# Wifi relay for fume extractor

## Features
* Designed for ESP8266 WIFI Wireless Relay Module AC DC Power Supply Single Relay.
* Designed to work with the JBC CDB/CDS soldering station.
* Checks the activity on UART line, to determine if the extractor is ON.
* When the soldering iron is taken out, it starts the fume extractor.
* When the soldering iron is put back in, it turns the fume extractor off, after a 10s period.
* Checks for time, auto turn off before 8 am and after 10 pm and on Sundays.
* Has a webserver, where you can turn it On or Off (seperately from the iron position). Ip can be found on the router or via the USB Serial terminal.
* Can be updated via the website. Upload firmware.bin to http://[localip]/_ac/update .
* Change the settings in the config.h, to set up the AP name, password, option to password protect the update page, set the page username and password.
* Compilable in VSCode, with Platformio, ESP12e.
