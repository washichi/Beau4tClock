#include "timekeeper.h"
#include <sunset.h>
#include "ledstrip.h"
#include "web.h"
#include "forecast.h"
#include "globals.h"

// #define SHOW_MINUTE_HAND

unsigned long lastNtpUpdateTime = ntpUpdatedateInterval_ms;
unsigned long lastClockUpdateTime = clockUpdateInterval_ms;
unsigned long lastForecastUpdateTime = forecastUpdateInterval_ms;

int utcOffsetHours = 2;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * utcOffsetHours);

SunSet sun;

void timekeeper_init()
{
  timeClient.begin();
  delay(250);
  while (timeClient.update() == false)
  {
    if (!timeClient.forceUpdate())
    {
      Serial.print(F("_ "));
      delay(10 * 1000);
    }
  }
  Serial.println();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(F(", "));
  Serial.print(timeClient.getHours());
  Serial.print(F(":"));
  Serial.print(timeClient.getMinutes());
  Serial.print(F(":"));
  Serial.print(timeClient.getSeconds());

  sun.setTZOffset(utcOffsetHours);
  int sunrise = static_cast<int>(sun.calcSunrise());
  int sunset = static_cast<int>(sun.calcSunset());
  Serial.println();
  Serial.print(F("Sunrise at "));
  Serial.print(sunrise / 60);
  Serial.print(F(":"));
  Serial.print(twoDigits(sunrise % 60));
  Serial.print(F("am, Sunset at "));
  Serial.print(sunset / 60);
  Serial.print(F(":"));
  Serial.print(twoDigits(sunset % 60));
  Serial.println(F("pm\n"));
}

void updateClock()
{
  time_t epochTime = timeClient.getEpochTime();
  const struct tm *ptm = gmtime((time_t *)&epochTime);
  int currentDay = ptm->tm_mday;
  int currentHour = ptm->tm_hour;
  int currentMinute = ptm->tm_min;

  // map next 12h to correct clockHour
  const int *knotsNext12h = getKnotsNext12h(currentDay, currentHour);
  int ordered12hForecast[12];
  for (int i = 0; i < 12; i++)
  {
    ordered12hForecast[i] = knotsNext12h[((NUM_HOURS - currentHour) + (i + 1)) % 12];
  }

  projectForecastColors(ordered12hForecast);
  // projectForecastColors(getKnotsNext12h(currentDay, currentHour));

  //  newValue = (newValueMax - newValueMin) * (value - valueMin) / (valueMax - valueMin) + newValueMin;
  int current12hTimeInMinutes = ((currentHour % 12) * 60) + currentMinute;
  int ledIndexHour = floor(((NUM_LEDS - 1) * current12hTimeInMinutes) / 719);
  // int ledIndexHour = round((currentHour % 12) * LEDS_PER_HOUR) - 2;
  // int ledIndexMinute = round(currentMinute * (NUM_LEDS-1) / 59) - 1;
  int ledIndexMinute = floor(((NUM_LEDS - 1) * currentMinute) / 59);
  leds[ledIndexHour] = CRGB::White;
#ifdef SHOW_MINUTE_HAND
  leds[ledIndexMinute] = CRGB::Red;
#endif

  if (config.dimWithSun)
  {
    int sunrise = static_cast<int>(sun.calcSunrise());
    int sunset = static_cast<int>(sun.calcSunset());
    int sunRiseHour = sunrise / 60;
    int sunRiseMinute = sunrise % 60;
    int sunSetHour = sunset / 60;
    int sunSetMinute = sunset % 60;

    int currentTimeInMinutes = (currentHour * 60) + currentMinute; // 24h
    int sunriseTimeInMinutes = (sunRiseHour * 60) + sunRiseMinute;
    int sunsetTimeInMinutes = (sunSetHour * 60) + sunSetMinute;
    Serial.println("currentTimeInMinutes = " + String(currentTimeInMinutes));
    Serial.println("sunriseTimeInMinutes = " + String(sunriseTimeInMinutes));
    Serial.println("sunsetTimeInMinutes = " + String(sunsetTimeInMinutes));

    int nightBrightness = 5;
    config.brightnessPercentage = 50;

    if (currentTimeInMinutes > sunriseTimeInMinutes && currentTimeInMinutes < sunsetTimeInMinutes)
    {
      //DAY, increase slowly to day brightness
      while (FastLED.getBrightness() < round(config.brightnessPercentage * 2.55))
      {
        Serial.println(FastLED.getBrightness());
        FastLED.setBrightness(FastLED.getBrightness() + 1);
        FastLED.show();
        FastLED.delay(100);
      }
    }
    else
    {
      //NIGHT, decrease slowly to night brightness
      while (FastLED.getBrightness() > nightBrightness)
      {
        FastLED.setBrightness(FastLED.getBrightness() - 1);
        FastLED.show();
        FastLED.delay(100);
      }
    }
  }

  FastLED.show();
  FastLED.show();

  Serial.print(currentHour);
  Serial.print(F("h, \tledIndex "));
  Serial.println(ledIndexHour);
  Serial.print(currentMinute);
  Serial.print(F("min, \tledIndex "));
  Serial.println(ledIndexMinute);
  Serial.println();
}

// utility function for digital clock display: prints leading 0
static String twoDigits(int digits)
{
  if (digits < 10)
  {
    String i = '0' + String(digits);
    return i;
  }
  else
  {
    return String(digits);
  }
}
