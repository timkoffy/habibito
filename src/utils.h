#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "config.h"

#define SECONDS_IN_DAY 86400
#define DAYS_IN_WEEK 7

extern volatile bool running;
void interrupt_handler(int signum);
struct winsize get_screen_size();
void setup_terminal();
void move_cursor(int x, int y);

int max_label_width(char **labels, int labels_count);

time_t ceil_timestamp_day(time_t timestamp);
char get_char_from_weekday(int weekday_idx);
int get_weekday_from_timestamp(time_t *timestamp);

void print_error(const char *err);

void change_bit(uint8_t *byte, int bit_idx);
int is_bit_true(uint8_t *byte, int bit_idx);
