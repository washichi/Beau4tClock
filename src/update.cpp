#include "update.h"
#include "Arduino.h"

static void update_started()
{
  Serial.println("CALLBACK:  HTTP update process started");
}

static void update_finished()
{
  Serial.println("CALLBACK:  HTTP update process finished");
}

static void update_progress(int cur, int total)
{
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

static void  update_error(int err)
{
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}