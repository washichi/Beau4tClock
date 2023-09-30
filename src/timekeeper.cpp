#include "timekeeper.h"
#include <sunset.h>
#include "ledstrip.h"
#include "web.h"
#include "forecast.h"

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
    if(!timeClient.forceUpdate()){
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
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int currentDay = ptm->tm_mday;
  int currentHour = ptm->tm_hour;
  int currentMinute = ptm->tm_min;

  projectForecastColors(getKnotsNext12h(currentDay, currentHour));  

  int ledIndexHour = round((currentHour % 12) * LEDS_PER_HOUR) - 2;
  int ledIndexMinute = round(currentMinute * NUM_LEDS / 59);
  leds[ledIndexHour] = CRGB::White;
  leds[ledIndexMinute] = CRGB::White;
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
