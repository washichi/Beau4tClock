#ifndef GLOBALS_H
#define GLOBALS_H

#define FIRMWARE_VERSION "0.0.3" 

// #define MATRIX_ENABLED  //Matrix module added that shows 3 day forecast and wind direction
// #define PIR_ENABLED     //PIR sensor added that enables matrix interaction 

#define PIN_LED LED_BUILTIN
#define PIN_LEDSTRIP_DI D1
#define PIN_LEDSTRIP_BI D2

typedef struct
{
  const char* provider;
  const char* spot;
  double lat;     
  double lon;
  bool dimWithSun;
  int dayBrightness;    //percentage ( 0 - 100)
  int nightBrightness; //percentage ( 0 - 100)
  bool handsOn; //OFF = not visible, ON = handsColor (white/color/black(leds off))
  int handsColor;
  int offThreshold; //threshold value in knots, below this value the led will turn off, instead of displaying knots color.
} config_t;

extern config_t config;


#endif