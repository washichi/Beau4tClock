#include "web.h"
#include "storage.h"
#include "globals.h"
#include "debug.h"
#include "ledstrip.h"

#define PORTAL_TIMEOUT_SEC 60

WiFiManager wm;
AsyncWebServer server(80);

const char *PARAM_MESSAGE = "status";
const char *PARAM_INPUT = "value";

String web_init()
{
  WiFi.mode(WIFI_AP_STA); // explicitly set mode, esp defaults to STA+AP
  // wm.resetSettings();
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);
  wm.setDebugOutput(false);

  wifi_station_set_hostname("Beau4tClock");
  WiFi.hostname("Beau4tClock");
  WiFi.setHostname("Beau4tClock");

  if (!wm.autoConnect("Beau4tClock", "12345678"))
  {
    Serial.println(F("Failed to connect"));
    //  ESP.restart();
  }

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

  server_init();

  return WiFi.localIP().toString();
}

void server_init()
{
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html", false, processor); });

  // Route for root style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/style.css", "text/css"); });

  // Route for root index.js
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.js", "text/javascript"); });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/favicon.ico", "image/x-icon"); });

  server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/manifest.json", "application/json"); });

  server.on("/apple-touch-icon.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/apple-touch-icon.png", "image/png"); });

  server.on("/android-chrome-512x512.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/android-chrome-512x512.png", "image/png"); });

  server.on("/browserconfig.xml", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/browserconfig.xml", "application/xml"); });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/fc-brightness", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage;
              // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
              if (request->hasParam(PARAM_INPUT)) {
                inputMessage = request->getParam(PARAM_INPUT)->value();
                String sliderValue = inputMessage;
                Serial.println(sliderValue.toInt());
              }
              else {
                inputMessage = "No message sent";
              }
              Serial.println(inputMessage);
              request->send(200, "text/plain", "OK"); });

  server.onNotFound(notFound);

  // Start server
  server.begin();

  /*
  // Route for root index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html", false, processor); });

  // Route for root index.css
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.css", "text/css"); });

  // Route for root entireframework.min.css
  server.on("/entireframework.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/entireframework.min.css", "text/css"); });

  // Route for root index.js
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.js", "text/javascript"); });

  // Respond to toggle event
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        String status;
        if (request->hasParam("status")) {
            status = request->getParam("status")->value();
            if(status == "ON"){
              Serial.println(true);
            }else{
              Serial.println(false);
            }
        } else {
            status = "No message sent";
        }
        request->send(200, "text/plain", "Turning Built In LED : " + status); });

  // brightness slider
  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  String inputMessage;
  // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
  if (request->hasParam(PARAM_INPUT)) {
    inputMessage = request->getParam(PARAM_INPUT)->value();
    config.brightnessPercentage = inputMessage.toInt();
    FastLED.setBrightness(round(config.brightnessPercentage * 2.55));
    FastLED.show();
    FastLED.show();


  }
  request->send(200, "text/plain", "OK"); });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/setSpot", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  String inputMessage;
  if (request->hasParam(PARAM_INPUT)) {
    inputMessage = request->getParam(PARAM_INPUT)->value();
    //@todo first validate if valid latlon
    String spotLatLonText = inputMessage;
    //Serial.print(F("spotLatLon = "));
    //Serial.println(spotLatLonText);
    //@todo validate if number & then update var lat lon
  }
  else {
    inputMessage = "No message sent";
  }
  //Serial.println(inputMessage);
  request->send(200, "text/plain", "OK"); });

  server.onNotFound(notFound);
  server.begin();
  */
}

static String processor(const String &var)
{
  if (var == "FC-BRIGHTNESS")
  {
    return String(config.brightnessPercentage);
  }
  else if (var == "SPOTNAME")
  {
    return config.spot;
  }
  else if (var == "SPOTLATLON")
  {
    return (String(config.lat) + ", " + String(config.lon));
  }

  return String();
}

static void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
