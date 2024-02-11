#include "forecast.h"
#include <ArduinoJson.h>
#include "web.h"
#include "globals.h"
#include "secrets.h"

forecast_type_t forecast;

void forecast_init()
{
    forecast.provider = "windfinder";
    forecast.spot = "Maasvlakte";
    forecast.spotId = 4879;
    for (int i = 0; i < NUM_DAYS; i++)
    {
        forecast.days[i].day = i + 30;
        for (int j = 0; j < (NUM_DAYS * NUM_HOURS); j++)
        {
            forecast.days[i].hours[j % NUM_HOURS].hour = (j % NUM_HOURS);
            forecast.days[i].hours[j % NUM_HOURS].windspeed = (j * 2) + 12;
            forecast.days[i].hours[j % NUM_HOURS].winddirectionLetters = "YX";
        }
    }
    // printForecast();
}

int getChunkyForecast()
{
    // Connect to server
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    WiFiClientSecure client;
    client.setTimeout(60 * 1000);
    client.setSSLVersion(BR_TLS10);
    client.setInsecure(); // This is for testing purposes, you should use a secure connection

    Serial.print(F("free heap: "));
    Serial.println(ESP.getFreeHeap());

    client.connect(SERVER_HOSTNAME, 443);

    // Serial.println(config.brightnessPercentage);
    // const char *provider = "windfinder";
    // const char *spot = "Maasvlakte";
    //@todo also get from json, in a elegant way.
    forecast.provider = config.provider;
    forecast.spot = config.spot;

    // HTTPS GET request
    char request[128] = "/data?provider=";
    strlcat(request, forecast.provider, sizeof(request));
    strlcat(request, "&spotname=", sizeof(request));
    strlcat(request, forecast.spot, sizeof(request));
    client.println("GET " + String(request) + " HTTP/1.0");
    client.println("Host: " + String(SERVER_HOSTNAME));
    client.println("Connection: close");
    client.println();

    client.find("\"provider\":");
    client.find("\"days\":");
    client.find("[");

    /*
        StaticJsonDocument<128> filter;
        // filter["provider"] = true;
        // filter["spot"] = true;
        filter["date"] = true;
        JsonObject filter_hours = filter["hours"].createNestedObject();
        filter_hours["hour"] = true;
        filter_hours["windspeed"] = true;
        filter_hours["winddirectionLetters"] = true;
    */

    int dayIndex = 0;
    int hourIndex = 0;
    do
    {
        // Deserialize the response
        DynamicJsonDocument doc(1024 * 4);
        DeserializationError error = deserializeJson(doc, client); //, DeserializationOption::Filter(filter));
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return EXIT_FAILURE;
        }
        Serial.print(F("free heap: "));
        Serial.println(ESP.getFreeHeap());

        // forecast.days[dayIndex].date = doc["date"].as<const char *>();
        sscanf(doc["date"].as<const char *>(), "%*[^,], %*s %d", &forecast.days[dayIndex].day);

        JsonArray hours = doc["hours"];
        for (JsonObject hour : hours)
        {
            forecast.days[dayIndex].hours[hourIndex % NUM_HOURS].hour = hour["hour"];
            forecast.days[dayIndex].hours[hourIndex % NUM_HOURS].windspeed = hour["windspeed"];
            forecast.days[dayIndex].hours[hourIndex % NUM_HOURS].winddirectionLetters = hour["winddirectionLetters"];

            hourIndex++;
        }
        dayIndex++;
    } while (client.findUntil(",", "]"));
    client.stop();

    Serial.print(String(hourIndex) + "Hours counted, for the " + String(dayIndex) + " dates: ");
    for (int i = 0; i < dayIndex; i++)
    {
        Serial.print(String(forecast.days[i].day) + ", ");
    }
    Serial.println();

    return EXIT_SUCCESS;
}

int *getKnotsNext12h(int currentDay, int currentHour)
{
    static int knotsNext12h[12];

    int knotsAllHours[NUM_HOURS * NUM_DAYS];
    int allHoursIndex = 0;
    int currentHourIndex = 0;
    for (int i = 0; i < NUM_DAYS; i++)
    {
        // int day;
        // sscanf(forecast.days[i].date, "%*[^,], %*s %d", &day);
        for (int j = 0; j < NUM_HOURS; j++)
        {
            knotsAllHours[allHoursIndex] = forecast.days[i].hours[j].windspeed;
            // Serial.println("CurrentDay = " + String(currentDay) + ", CurrentHour = " + String(currentHour));
            // Serial.println("ForecastDay = " + String(forecast.days[i].day) + ", ForecastHour = " + String(forecast.days[i].hours[j].hour));
            if (currentDay == forecast.days[i].day && currentHour == forecast.days[i].hours[j].hour)
            {
                currentHourIndex = allHoursIndex;
            }
            allHoursIndex++;
        }
    }

    Serial.print(F("knotsAllHours:"));
    for (int i = 0; i < (NUM_DAYS * NUM_HOURS); i++)
    {
        if (i % NUM_HOURS == 0)
        {
            Serial.println();
        }
        Serial.print(knotsAllHours[i]);
        Serial.print(", ");
    }
    Serial.println();

    Serial.println("CurrentDay = " + String(currentDay) + ", CurrentHour = " + String(currentHour));
    // Serial.println("currentHourIndex = " + String(currentHourIndex));

    Serial.print(F("knotsNext12Hours:\t"));
    for (int i = 0; i < 12; i++)
    {
        knotsNext12h[i] = knotsAllHours[currentHourIndex + i]; //(i * 2) + 12;
        Serial.print(knotsNext12h[i]);
        Serial.print(", ");
    }
    Serial.println();

    return knotsNext12h;
}

void printForecast()
{
    Serial.println(F("::::: FORECAST :::::"));
    Serial.print(F("Provider:\t"));
    Serial.println(forecast.provider);
    Serial.print(F("Spot:\t\t"));
    Serial.println(forecast.spot);

    Serial.println(F("Date\t\tHour\t\tKnots\t\tColor"));
    Serial.println(F("---------------------------------------------------"));

    for (int i = 0; i < NUM_DAYS; i++)
    {
        for (int j = 0; j < NUM_HOURS; j++)
        {
            delay(50);
            Serial.print(forecast.days[i].day);
            delay(50);
            Serial.print(F("\t"));
            Serial.print(forecast.days[i].hours[j].hour);
            Serial.print(F("\t\t"));
            Serial.print(forecast.days[i].hours[j].windspeed);
            Serial.print(F("\t\t"));
            // Serial.print(colorMap[forecast.days[i].hours[j].windspeed]);
            Serial.println();
            delay(50);
        }
        Serial.println();
    }
}