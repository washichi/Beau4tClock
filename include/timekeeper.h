#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include <Arduino.h>
#include <NTPClient.h>

const unsigned long clockUpdateInterval_ms = 60 * 1000;             // Update clock every 60 seconds
const unsigned long forecastUpdateInterval_ms = 1 * 60 * 60 * 1000; // Update forecast every hour (3600000 milliseconds)
const unsigned long ntpUpdatedateInterval_ms = 24 * 60 * 60 * 1000; // every 24h

extern unsigned long lastClockUpdateTime;
extern unsigned long lastForecastUpdateTime;
extern unsigned long lastNtpUpdateTime;

extern NTPClient timeClient;

void timekeeper_init();
void updateClock();

//static String twoDigits(int digits);

#endif