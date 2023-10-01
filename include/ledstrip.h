#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>

#define NUM_LEDS 36
#define LEDS_PER_HOUR (NUM_LEDS / 12)
#define LEDS_PER_MINUTE (NUM_LEDS / 60)

extern CRGB leds[NUM_LEDS];

void ledstrip_init();
void blinkIP(String ipAddress);
void ledBootAnimation();
void projectForecastColors(int knotsNext12h[]);
void ledFadeOut(int fadeBy, int delayMs);

#endif