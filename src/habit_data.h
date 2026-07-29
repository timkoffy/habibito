#pragma once 

struct habit_data {
    int start_date; // unix timestamp
    unsigned short bytes_per_day;
    int days_count;
    unsigned short labels_count;
    unsigned short max_label_length;
    unsigned short labels_buffer_count;
    char **labels;
    unsigned char **data;
};
