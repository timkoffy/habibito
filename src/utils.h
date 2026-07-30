#pragma once 

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "config.h"

#define SECONDS_IN_DAY 86400

extern volatile short running;
struct winsize get_screen_size();
void setup_terminal();

unsigned short max_label_width(char **labels, unsigned short labels_count);

time_t ceil_timestamp_day(time_t timestamp);
char convert_weekday_idx_to_str(unsigned short wd_idx);
int get_weekday_from_timestamp(time_t *timestamp);
