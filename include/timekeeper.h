#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include <stdint.h>
#include <WiFiUdp.h>


#include "web.h"

extern unsigned long intervalNTP;
extern unsigned long prevNTP;

extern IPAddress timeServerIP; 


void timekeeper_init();
void startUDP();

void sendNTPpacket(IPAddress& address);

#endif