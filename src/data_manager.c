#include "data_manager.h"
#include "config.h"

struct habit_data load_mock_data() {
    struct habit_data data;
    data.start_date = 1609459200;     
    data.bytes_per_day = 8;             
    data.days_count = 24;              
    data.labels_count = 3;              
    data.labels_buffer_count = data.bytes_per_day * 8;

    data.labels = malloc(sizeof(char*) * data.labels_buffer_count);
    for (int i = 0; i < data.labels_buffer_count; i++) {
        if (i < data.labels_count) data.labels[i] = malloc(sizeof(char) * MAX_LABEL_LENGTH);
        else data.labels[i] = NULL;
    }

    data.data = malloc(sizeof(unsigned char*) * data.days_count);
    for(int i = 0; i < data.days_count; i++) {
        data.data[i] = malloc(sizeof(unsigned char) * data.bytes_per_day);
    }

    char str1[] = "linux";
    char str2[] = "mus";
    char str3[] = "stretching-awareness";

    memcpy(data.labels[0], str1, sizeof(str1)); 
    memcpy(data.labels[1], str2, sizeof(str2)); 
    memcpy(data.labels[2], str3, sizeof(str3)); 

    unsigned char tmp = (unsigned char)0x0000;

    for (int i = 0; i < data.days_count; i++) {
        memcpy((void*)data.data[i], (void*)&tmp, data.bytes_per_day);
    }

    return data;
}

void free_allocated_habit_data(struct habit_data data) {
    for (int i = 0; i < data.labels_buffer_count; i++) {
        if (data.labels[i] != NULL) free(data.labels[i]);
    }
    free(data.labels);

    for(int i = 0; i < data.days_count; i++) {
        free(data.data[i]);
    }
    free(data.data);
}
