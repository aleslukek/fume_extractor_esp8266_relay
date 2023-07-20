#ifndef WIFI_CPP
#define WIFI_CPP

#include <setup.h>

void wifi_setup() {
    server.on("/", handleRoot);
    server.on("/io", handleGPIO);
    config.apid = AP_NAME;
    config.psk = AP_PASS;
    // config.apid = ESP.hostname();          // Retrieve host name to SotAp identification
    config.apip = IPAddress(99, 99, 99, 99);  // Sets SoftAP IP address
    config.autoReconnect = true;              // Enable auto-reconnect
    config.autoSave = AC_SAVECREDENTIAL_AUTO; // No save credential
    // config.boundaryOffset = 64;            // Reserve 64 bytes for the user data in EEPROM.
    config.portalTimeout = 300000;            // Sets timeout value for the captive portal
    config.retainPortal = false;              // Retains the portal function after timed-out
    config.homeUri = "/";                     // Sets home path of Sketch application
    config.title = PAGE_TITLE;                // Customize the menu title
    config.menuItems = AC_MENUITEM_UPDATE | AC_MENUITEM_CONFIGNEW | AC_MENUITEM_OPENSSIDS | AC_MENUITEM_DISCONNECT | AC_MENUITEM_RESET | AC_MENUITEM_HOME;
    config.homeUri = "/";
#if ENABLE_PASSWORD_PROTECTED_WEBSITE == 1
    config.username = ADMIN_PAGE_USERNAME;
    config.password = ADMIN_PAGE_PASSWORD;
    config.auth = AC_AUTH_DIGEST;
    config.authScope = AC_AUTHSCOPE_PORTAL;
#endif
    config.ota = AC_OTA_BUILTIN;
    portal.config(config);
    portal.onOTAStart(exitOTAStart);
    portal.onOTAEnd(exitOTAEnd);
    portal.onOTAProgress(exitOTAProgress);
    portal.onOTAError(exitOTAError);
    if (portal.begin()) {
        Serial.println("Portal started, IP:" + WiFi.localIP().toString());
    };
    portal.onDetect(atDetect);
    return;
}

bool atDetect(IPAddress &softapIP) {
    Serial.println("Captive portal started, SoftAP IP:" + softapIP.toString());
    return true;
}

void handleRoot() {
    String page = PSTR(
        "<html>"
        "<head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style type=\"text/css\">"
        "body {"
        "-webkit-appearance:none;"
        "-moz-appearance:none;"
        "font-family:'Arial',sans-serif;"
        "text-align:center;"
        "}"
        ".menu > a:link {"
        "position: absolute;"
        "display: inline-block;"
        "right: 12px;"
        "padding: 0 6px;"
        "text-decoration: none;"
        "}"
        ".button {"
        "display:inline-block;"
        "border-radius:7px;"
        "background:#73ad21;"
        "margin:0 10px 0 10px;"
        "padding:10px 20px 10px 20px;"
        "text-decoration:none;"
        "color:#000000;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_32) "</div>");

    page += ("<br>");
#ifdef SHOW_TIME
    page += days[time_day];
    page += (", ");
    page += String(time_hour);
    page += (":");
    page += String(time_min);
#endif
    page += ("<br>Fume extractor ");
    page += String(F(": <span style=\"font-weight:bold;color:"));
    page += digitalRead(relay_pin) ? String("Tomato\">ON") : String("SlateBlue\">OFF");
    page += String(F("</span>"));
    page += String(F("<p><a class=\"button\" href=\"/io?v=Turn OFF\">Turn OFF</a><a class=\"button\" href=\"/io?v=Turn ON\">Turn ON</a></p>"));
    page += String(F("<br>Firmware version: "));
    page += (FIRMWARE_VERSION);
    page += String(F("<br><a href=\"/_ac/update\">Upload a new firmware</a>"));
    page += String(F("</body></html>"));
    portal.host().send(200, "text/html", page);
}

void sendRedirect(String uri) {
    WebServer &server = portal.host();
    server.sendHeader("Location", uri, true);
    server.send(302, "text/plain", "");
    server.client().stop();
}

void handleGPIO() {
    WebServer &server = portal.host();
    if (server.arg("v") == "Turn OFF") {
        digitalWrite(relay_pin, LOW);
#if TOGGLE_ONBOARD_LED_FOR_TESTING == 1
        digitalWrite(station_status_led, SS_LED_NORMALY_LOW);
#endif
        relay_status = false;
        relay_command = false;
        Serial.println(F("Webapp relay turned off"));
    } else if (server.arg("v") == "Turn ON") {
        digitalWrite(relay_pin, HIGH);
#if TOGGLE_ONBOARD_LED_FOR_TESTING == 1
        digitalWrite(station_status_led, SS_LED_NORMALY_HIGH);
#endif
        relay_status = true;
        relay_command = true;
        Serial.println(F("Webapp relay turned on"));
    }
    sendRedirect("/");
}

void wifi_loop() {
    portal.handleClient();
    if (!print_local_ip) {
        print_local_ip = true;
        Serial.print(F("Local ip: "));
        Serial.println(WiFi.localIP());
    }
    return;
}

void exitOTAStart() {
    Serial.println(F("OTA started"));
}

void exitOTAProgress(unsigned int amount, unsigned int sz) {
    Serial.printf("OTA in progress: received %d bytes, total %d bytes\n", sz, amount);
}

void exitOTAEnd() {
    Serial.println(F("OTA ended"));
}

void exitOTAError(uint8_t err) {
    Serial.printf("OTA error occurred %d\n", err);
}

#endif