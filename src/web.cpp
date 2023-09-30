#include "web.h"
#include "debug.h"

#define PORTAL_TIMEOUT_SEC 60

WiFiManager wm;



String web_init()
{
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    wm.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);
    wm.setDebugOutput(false);

    WiFi.hostname("Beau4tClock");
    WiFi.setHostname("Beau4tClock");

    if (!wm.autoConnect("Beau4tClock", "12345678"))
    {
        Serial.println(F("Failed to connect"));
        //  ESP.restart();
    }
    //wm.resetSettings();

/*
    Serial.println("XXX -1- XXX");
    if (!wm.startConfigPortal("Beau4tClock", "12345678"))
    {
        Serial.println(F("failed to connect and hit timeout"));
        delay(3000);
        ESP.restart();
        delay(5000);
    }
    Serial.println("XXX -2- XXX");
 */

    Serial.println(F("\n\nconnected..."));
    Serial.print(F("Hostname: "));
    Serial.print(WiFi.hostname());
    Serial.print(F("   IP: "));
    Serial.println(WiFi.localIP());
    Serial.println();

    return WiFi.localIP().toString();
}

void server_init(){
}