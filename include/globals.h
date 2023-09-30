#ifndef GLOBALS_H
#define GLOBALS_H

#define FIRMWARE_VERSION "0.2"

#define MATRIX_ENABLED  //Matrix module added that shows 3 day forecast and wind direction
#define PIR_ENABLED     //PIR sensor added that enables matrix interaction 

#define PIN_LED LED_BUILTIN
#define PIN_LEDSTRIP_DI D1
#define PIN_LEDSTRIP_BI D2

typedef struct
{
  const char* provider;
  const char* spot;
  double lat;
  double lon;
  int brightnessPercentage;
  bool dimWithSun;
  bool handsOn; //OFF = not visible, ON = handsColor (white/color/black(leds off))
  int handsColor;
} config_t;

extern config_t config;

#endif