#ifndef FORECAST_H
#define FORECAST_H

#define NUM_DAYS 3
#define NUM_HOURS 24

typedef struct
{
  int hour;
  int windspeed;
  const char *winddirectionLetters;
} hour_type_t;

typedef struct
{
  const char *date;
  hour_type_t hours[24];
} day_type_t;

typedef struct
{
  const char* provider;
  const char* spot; // spotname for windfinder, spotid for windguru
  int spotId;
  day_type_t days[3];
} forecast_type_t;

int getChunkyForecast();
int* getKnotsNext12h(int currentDay, int currentHour);
void printForecast();

#endif