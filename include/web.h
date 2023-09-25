#ifndef WEB_H
#define WEB_H


#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#define WEBSERVER_H
#include "ESPAsyncWebServer.h"

void web_init(void);

#endif