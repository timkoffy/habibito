#include "data_manager.h"

struct habit_data load_mock_data() {
    data.start_date = 1609459200;     
    data.bytes_per_day = 8;             
    data.days_count = 3;              
    data.labels_count = 3;              
    data.max_label_length = 20;       
    data.labels_buffer_count = data.bytes_per_day * 8;

    data.labels = malloc(sizeof(char*) * data.labels_buffer_count);
    for (int i = 0; i < data.labels_buffer_count; i++) {
        if (i < data.labels_count) data.labels[i] = malloc(sizeof(char) * data.max_label_length);
        else data.labels[i] = NULL;
    }

    data.data = malloc(sizeof(unsigned char*) * data.days_count);
    for(int i = 0; i < data.days_count; i++) {
        data.data[i] = malloc(sizeof(unsigned char) * data.bytes_per_day);
    }

    data.labels[0] = "linux";
    data.labels[1] = "music";
    data.labels[2] = "shower";

    data.data[0] = 0x0000;
    data.data[1] = 0x0000;
    data.data[2] = 0x0001;

    return data;
}
