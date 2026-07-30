#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "config.h"

#define SECONDS_IN_DAY 86400
#define DAYS_IN_WEEK 7
 
extern volatile short running;
struct winsize get_screen_size();
void setup_terminal();
void move_cursor(int x, int y);

unsigned short max_label_width(char **labels, unsigned short labels_count);

time_t ceil_timestamp_day(time_t timestamp);
char get_char_from_weekday(unsigned short wd_idx);
int get_weekday_from_timestamp(time_t *timestamp);
