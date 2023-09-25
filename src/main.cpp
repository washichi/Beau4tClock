#include <Arduino.h>
#include "globals.h"
#include "web.h"
#include "forecast.h"
#include "ledstrip.h"
#include "timekeeper.h"

#define FIRMWARE_VERSION "0.2"

void setup()
{
    Serial.begin(115200);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, !true);

    Serial.print(F("\n---------------------------------------------------"));
    Serial.print(F("\nFirmware version:\t"));
    Serial.println(FIRMWARE_VERSION);
    Serial.print(F("\nChipID:\t\t"));
    Serial.println(ESP.getChipId());
    Serial.print(F("---------------------------------------------------\n"));

    web_init();
    timekeeper_init();
    //ledstrip_init();
    //storage_init();
    //forecast_init();

    digitalWrite(PIN_LED, !false);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - prevNTP > intervalNTP)
    { // If last NTP request inteval has passed
        prevNTP = currentMillis;
        Serial.println("\r\nSending NTP request ...");
        sendNTPpacket(timeServerIP); // Send an NTP request
    }

    delay(1000);
}