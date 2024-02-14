#include "web.h"
#include "storage.h"
#include "globals.h"
#include "debug.h"
#include "ledstrip.h"
#include "update.h"
#include "timekeeper.h"

#define PORTAL_TIMEOUT_SEC 5 * 60

#include "debug.h"

WiFiManager wm;
AsyncWebServer server(80);

const char *PARAM_MESSAGE = "status";
const char *PARAM_INPUT = "value";

static String processor(const String &var);
static void notFound(AsyncWebServerRequest *request);

String web_init()
{
  WiFiManager wm;

  wifi_station_set_hostname("Beau4tClock");
  wm.setHostname("Beau4tClock");
  WiFi.hostname("Beau4tClock");
  WiFi.setHostname("Beau4tClock");

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP //WIFI_AP_STA
  wm.setDebugOutput(true);
  // wm.resetSettings();

  if (!wm.autoConnect("Beau4tClock", "12345678"))
  {
    wm.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);
    if (!wm.startConfigPortal("Beau4tClock", "12345678"))
    {
      if (!WiFi.isConnected())
      {
        Serial.println(F("Portal timed out, and no valid WiFi configuration to connect to."));
        Serial.println(F("Rebooting . . . "));
        delay(3000);
        ESP.restart();
      }
    }
  }

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

  // Route for root entireframework.min.css
  server.on("/entireframework.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/entireframework.min.css", "text/css"); });

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

  // sliderFcBrMin ###############################################################
  server.on("/sliderFcBrMin", HTTP_GET, [](AsyncWebServerRequest *request)
            {          
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      config.nightBrightness = inputMessage.toInt();
      updateBrightness();
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK"); });

  server.on("/getSliderValueFcBrMin", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(config.nightBrightness)); });
  // #############################################################################

  // sliderFcBrMax ###############################################################
  server.on("/sliderFcBrMax", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      config.dayBrightness = inputMessage.toInt();
      updateBrightness();
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK"); });

  server.on("/getSliderValueFcBrMax", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(config.dayBrightness)); });
  // ###############################################################################

  // sliderKnots #################################################################
  server.on("/sliderKnots", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      config.offThreshold = inputMessage.toInt();
      updateClock();
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK"); });

  server.on("/getSliderValueKnots", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(config.offThreshold)); });
  // ###############################################################################

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(200, "text/plain", "OK"); 
              updateFirmware(); 
             updateFileSystem(); });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              Serial.println("/reset received");
              digitalWrite(0, LOW); //trigger RST //@todo doesnt trigger reeset function. 
            request->send(200, "text/plain", "OK"); });

  server.onNotFound(notFound);
  server.begin();
}

static String processor(const String &var)
{
  D_println("::::::::::: processor : " + var);
  if (var == "SLIDERVALUEFCBRMIN")
  {
    return String(config.nightBrightness);
  }
  else if (var == "SLIDERVALUEFCBRMAX")
  {
    return String(config.dayBrightness);
  }
  else if (var == "SLIDERVALUEKNOTS")
  {
    return String(config.offThreshold);
  }
  else if (var == "SPOTNAME")
  {
    return String(config.spot);
  }
  else if (var == "SPOTLATLON")
  {
    return (String(config.lat) + ", " + String(config.lon));
  }
  else if (var == "PROVIDER")
  {
    return String(config.provider);
  }

  return String();
}

static void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void web_process()
{
  wm.process();
}

void reset_web()
{
  WiFi.disconnect(true);
  wm.resetSettings();

  // Remove WiFi credentials
  ESP.eraseConfig();
}