#include "timekeeper.h"

WiFiUDP UDP;     

// ###NTP and time config###
unsigned long timeZone = 1.0;                  // Change this value to your local timezone (in my case +1 for Amsterdam)
const char *NTPServerName = "nl.pool.ntp.org"; // Change this to a ntpserver nearby, check this site for a list of servers: https://www.pool.ntp.org/en/
unsigned long intervalNTP = 24 * 60 * 60 * 1000;      // Request a new NTP time every 24 hours
unsigned long NTPmaxWait = 60 * 1000; // Maximum Time to wait for NTP request

IPAddress timeServerIP;
const int NTP_PACKET_SIZE = 48;
uint8_t NTPBuffer[NTP_PACKET_SIZE];

unsigned long prevNTP = intervalNTP;//0;//ULONG_MAX - 1;
unsigned long lastNTPResponse = millis();
uint32_t timeUNIX = 0;
unsigned long prevActualTime = 0;
unsigned long lastMillis = millis();
unsigned long lastSecondSwitch = 0; // check if needed

int currentSecond = 0;
int currentMinute = 0;
int currentHour = 0;

#define LEAP_YEAR(Y) (((1970 + Y) > 0) && !((1970 + Y) % 4) && (((1970 + Y) % 100) || !((1970 + Y) % 400)))
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct DateTime
{
    int year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayofweek;
};

DateTime currentDateTime;

////////////////


void timekeeper_init(){
    startUDP();
}

void startUDP() {
  UDP.begin(123);                          // Start listening for UDP messages on port 123
  //Serial.print(F("\nLocal  UDP port:\t"));
  //Serial.println(UDP.localPort());
}

void sendNTPpacket(IPAddress &address)
{
    memset(NTPBuffer, 0, NTP_PACKET_SIZE); // set all bytes in the buffer to 0
    // Initialize values needed to form NTP request
    NTPBuffer[0] = 0b11100011; // LI, Version, Mode
    // send a packet requesting a timestamp:
    UDP.beginPacket(address, 123); // NTP requests are to port 123
    UDP.write(NTPBuffer, NTP_PACKET_SIZE);
    UDP.endPacket();
}