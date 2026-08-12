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


/* ===== TERMINAL UTILS ===== */

void interrupt_handler(int signum);

struct winsize get_screen_size();

/* Prepare terminal for TUI application:
 *     - turn off canonical input (waiting newline character), 
 *     - turn off echo, 
 *     - make cursor invisible,
 *     - handle SIGINT, SIGTERM via interrupt_handler().
 * */
void setup_terminal();

/* Move cursor using ESC sequence.
 *
 * Equal to printf("\e[%d;%dH", row, col).
 * */
void move_cursor(int row, int col);


/* ===== VISUAL UTILS ===== */

/* Return max length of labels.
 *
 * If result less than MIN_LABEL_WIDTH it returns MIN_LABEL_WIDTH.
 * If result greater or equal to MAX_LABEL_LENGHTH it returns MAX_LABEL_LENGTH.
 * */
int max_label_width(char **labels, int labels_count);


/* ===== TIME UTILS ===== */

time_t ceil_timestamp_day(time_t timestamp);

char get_char_from_weekday(int weekday_idx);

int get_weekday_from_timestamp(time_t *timestamp);


/* ===== LOG UTILS ===== */

void print_error(const char *err);


/* ===== DATA UTILS ===== */

void change_bit(uint8_t *byte, int bit_idx);

int is_bit_true(uint8_t *byte, int bit_idx);
