#ifndef WEB_H
#define WEB_H


#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#define WEBSERVER_H
#include "ESPAsyncWebServer.h"


#ifndef DEBUG
    #define PORTAL_TIMEOUT_SEC 60
#endif

String web_init(void);

#endif