#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG

#ifdef DEBUG

#include <Arduino.h>
#include <stdarg.h>

#define PORTAL_TIMEOUT_SEC 10
#define BUTTON_ENABLED
#define PIN_BUTTON D3

#define D_SerialBegin(...) Serial.begin(__VA_ARGS__);
#define D_print(...) Serial.print(__VA_ARGS__)
#define D_write(...) Serial.write(__VA_ARGS__)
#define D_println(...) Serial.println(__VA_ARGS__)
#else
#define D_SerialBegin(bauds)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

#endif