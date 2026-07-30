#pragma once 

struct habit_data {
    time_t start_time; // unix timestamp
    time_t current_time;
    unsigned short bytes_per_day;
    int days_count;
    unsigned short labels_count;
    unsigned short labels_buffer_count;
    char **labels;
    unsigned char **data;
};
