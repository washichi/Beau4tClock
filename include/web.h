#ifndef WEB_H
#define WEB_H

#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#define WEBSERVER_H
#include "ESPAsyncWebServer.h"


// #ifndef DEBUG
//     #define PORTAL_TIMEOUT_SEC 30
// #endif

String web_init(void);
void server_init();

//static String processor(const String &var);
//static void notFound(AsyncWebServerRequest *request);

#endif