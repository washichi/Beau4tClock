#ifndef UPDATE_H
#define UPDATE_H

void updateFirmware();
void updateFileSystem();

static void update_started();
static void update_finished();
static void update_progress(int cur, int total);
static void  update_error(int err);

#endif