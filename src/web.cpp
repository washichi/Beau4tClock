#include "web.h"


#define PORTAL_TIMEOUT_SEC 60
//WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
WiFiManager wm;



void web_init()
{
    // wm.resetSettings();
    bool res;
    res = wm.autoConnect("Beau4tClock", "12345678");
    if (!res)
    {
        Serial.println(F("Failed to connect"));
        //  ESP.restart();
    }

    wifi_station_set_hostname("Beau4tClock");
    //wm.hostname("Beau4tClock");
    WiFi.setHostname("Beau4tClock");

    // wm.resetSettings();

    wm.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);
    wm.setDebugOutput(false);
    if (!wm.startConfigPortal("Beau4tClock", "12345678"))
    {
        Serial.println(F("failed to connect and hit timeout"));
        delay(3000);
        ESP.restart();
        delay(5000);
    }

    Serial.println(F("\n\nconnected..."));
    Serial.print(F("Hostname: "));
    Serial.print(WiFi.hostname());
    Serial.print(F("   IP: "));
    Serial.println(WiFi.localIP());
    Serial.println();
}