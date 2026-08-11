#pragma once 

#include <stdint.h>
#include <time.h>

struct habit_data {
    time_t start_time;
    time_t current_time;
    int bytes_per_day;
    int days_count;
    int labels_count;
    int labels_buffer_count;
    char **labels;
    uint8_t **data;
    char *art_buffer;
};
