#include <Arduino.h>
#include "globals.h"
#include "web.h"
#include "forecast.h"
#include "ledstrip.h"
#include "timekeeper.h"
#include "storage.h"
#include "debug.h"
#include "update.h"


config_t config = {
    .provider = "windfinder",
    .spot = "maasvlakte",
    .lat = 51.919,
    .lon = 3.984,
    .dimWithSun = true,
    .dayBrightness = 50,
    .nightBrightness = 0,
    .offThreshold = 12, 
};

void setup()
{
    Serial.begin(115200);
    config.dayBrightness = 50;

#ifdef DEBUG
    pinMode(PIN_BUTTON, INPUT_PULLUP);
#endif

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, !true);

    Serial.print(F("\n---------------------------------------------------"));
    Serial.print(F("\nFirmware version:\t"));
    Serial.println(FIRMWARE_VERSION);
    Serial.print(F("ChipID:\t\t\t"));
    Serial.println(ESP.getChipId());
    Serial.print(F("---------------------------------------------------\n"));

    updateFirmware(); //@todo first connect to wifi?

    storage_init();
    ledstrip_init();
    String ipAdress = web_init();
    blinkIP(ipAdress);
    timekeeper_init();

    // forecast_init();

    digitalWrite(PIN_LED, !false);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - lastNtpUpdateTime >= ntpUpdatedateInterval_ms)
    {
        Serial.println("[[[ NtpUpdate ]]]");
        lastNtpUpdateTime = currentMillis;
        timeClient.update();
    }

    if (currentMillis - lastForecastUpdateTime >= forecastUpdateInterval_ms)
    {
        Serial.println("[[[ ForecastUpdate ]]]");
        lastForecastUpdateTime = currentMillis;
        int tries = 0;
        while (getChunkyForecast() != EXIT_SUCCESS)
        {
            if (tries >= 50)
            {
                Serial.println(F("Tried to get forecast 50 times. u no working."));
                ESP.restart();
            }
            tries++;
            FastLED.delay(10 * 1000);
        }
        // printForecast();
    }

    if (currentMillis - lastClockUpdateTime >= clockUpdateInterval_ms)
    {
        Serial.println("[[[ ClockUpdate ]]]");
        lastClockUpdateTime = currentMillis;
        updateClock();
    }

#ifdef BUTTON_ENABLED
    if (!digitalRead(PIN_BUTTON))
    {
        FastLED.clear();
        FastLED.show();
        FastLED.show();
        digitalWrite(PIN_LED, !true);
        getChunkyForecast();
        updateClock();
        digitalWrite(PIN_LED, !false);
    }
#endif
}