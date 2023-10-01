#include "ledstrip.h"
#include "globals.h"
#include "gpio.h"

CRGB leds[NUM_LEDS];

const char *colorMap[] = {
    "0x7900CC",  // 0
    "0x6400CC",  // 1
    "0x5000CC",  // 2
    "0x3C00CC",  // 3
    "0x2800CC",  // 4
    "0x1414CC",  // 5
    "0x0028CC",  // 6
    "0x003CCC",  // 7
    "0x0050CC",  // 8
    "0x0064CC",  // 9
    "0x0078CC",  // 10
    "0x008BCC",  // 11
    "0x009FCC",  // 12
    "0x00ABC6",  // 13
    "0x00B8C0",  // 14
    "0x0FA99A",  // 15
    "0x1E9A75",  // 16
    "0x16A241",  // 17
    "0x0EAA0E",  // 18
    "0x07B107",  // 19
    "0x00B800",  // 20
    "0x1DC000",  // 21
    "0x3BC800",  // 22
    "0x4EAD00",  // 23
    "0x619200",  // 24
    "0x7D9500",  // 25
    "0x999800",  // 26
    "0xA39200",  // 27
    "0xAE8C00",  // 28
    "0xB18200",  // 29
    "0xB57800",  // 30
    "0xA77000",  // 31
    "0x996900",  // 32
    "0xB27000",  // 33
    "0xCC7800",  // 34
    "0xC26E00",  // 35
    "0xB86400",  // 36
    "0xC46D0E",  // 37
    "0xD1771D",  // 38
    "0xC0591A",  // 39
    "0xB03B17",  // 40
    "0xA83117",  // 41
    "0xA02817",  // 42
    "0x981E17",  // 43
    "0x901417",  // 44
    "0x980A17",  // 45
    "0xA10017",  // 46
    "0x98001F",  // 47
    "0x900028",  // 48
    "0x98003C",  // 49
    "0xA00050"}; // 50

void ledstrip_init()
{
  //@todo get pin from ui >> config.pinLEDstrip
  FastLED.addLeds<WS2812B, PIN_LEDSTRIP_DI, GRB>(leds, NUM_LEDS);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(round(config.brightnessPercentage * 2.55));
  FastLED.clear(true);
  FastLED.show();
  FastLED.show();

  ledBootAnimation();
}

void blinkIP(String ipAddress)
{
  for (char x : ipAddress)
  {
    FastLED.clear();
    FastLED.show();
    FastLED.show();
    delay(500);

    if (x == '.')
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::White;
      }
      FastLED.setBrightness(20);
      FastLED.show();
      FastLED.show();
    }
    else
    {
      leds[(int(x - '0') * LEDS_PER_HOUR) - 1] = CRGB::White;
      FastLED.setBrightness(255);
      FastLED.show();
      FastLED.show();
    }
    delay(1500);
  }

  FastLED.setBrightness(round(config.brightnessPercentage * 2.55));
  FastLED.clear();
  FastLED.show();
  FastLED.show();
}

void ledBootAnimation()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int windspeed = round(i * 1.42857); // map windspeed colors to led
    leds[i] = strtol(colorMap[windspeed], NULL, 0);
    FastLED.show();
    FastLED.show();
    delay(50);
  }

  ledFadeOut(35, 50);
}

/**
 * Reduce the brightness of an array of pixels all at once.
 * This function will eventually fade all the way to black.
 * @param fadeBy how much to fade each LED
 * @param delayMs delay between fade steps
 */
void ledFadeOut(int fadeBy, int delayMs)
{
  for (int i = 1; i < fadeBy; i++)
  {
    fadeToBlackBy(leds, NUM_LEDS, i); // 64/255 = 25%
    FastLED.show();
    FastLED.show();
    delay(delayMs);
  }
}



void projectForecastColors(int knotsNext12h[])
{
  LEDS.clear();
  for (int i = 0; i < 12; i++)
  {
    int ledIndex = (((i + 1) * LEDS_PER_HOUR) - 2);
    Serial.print("Hour " + String(i+1));
    Serial.print("\t,ledIndex: " + String(ledIndex));
    Serial.print(",\twindspeed: " + String(knotsNext12h[i]));
    Serial.println();
    leds[ledIndex] = strtol(colorMap[knotsNext12h[i]], NULL, 0);
  }
}
