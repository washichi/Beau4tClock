#include "update.h"
#include "web.h"
#include "globals.h"
#include "secrets.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>


void updateFirmware()
{
  Serial.println(F(":::: updateFirmware ::::"));


  WiFiClientSecure client;
  client.setTimeout(60 * 1000);
  client.setSSLVersion(BR_TLS10);
  client.setInsecure(); // This is for testing purposes, you should use a secure connection

  // The line below is optional. It can be used to blink the LED on the board during flashing
  // The LED will be on during download of one buffer of data from the network. The LED will
  // be off during writing that buffer to flash
  // On a good connection the LED should flash regularly. On a bad connection the LED will be
  // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
  // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
  // httpUpdate.setLedPin(LED_BUILTIN, LOW);
  
  // Add optional callback notifiers
  ESPhttpUpdate.onStart(update_started);
  ESPhttpUpdate.onEnd(update_finished);
  ESPhttpUpdate.onProgress(update_progress);
  ESPhttpUpdate.onError(update_error);
  
  ESPhttpUpdate.rebootOnUpdate(false); // remove automatic update

  Serial.println(F("Update start now!"));

  //forecast.homespot.dev/update?chipid=580705&type=firmware&currentversion=0.0.3

  String updateUrl = "/update?chipid=" + String(ESP.getChipId()) + "&type=firmware&currentversion=" + FIRMWARE_VERSION;
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, SERVER_HOSTNAME, 443, updateUrl);


  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    Serial.println(F("Retry in 10secs!"));
    delay(10000); // Wait 10secs
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    delay(1000); // Wait a second and restart
    ESP.restart();
    break;
  }
}

void updateFileSystem(){

}

static void update_started()
{
  Serial.println("CALLBACK:  HTTP update process started");
}

static void update_finished()
{
  Serial.println("CALLBACK:  HTTP update process finished");
}

static void update_progress(int cur, int total)
{
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

static void update_error(int err)
{
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}