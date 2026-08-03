#include "data_manager.h"

struct habit_data load_mock_data(time_t current_time) {
    struct habit_data data;
    data.start_time = ceil_timestamp_day(1785605978);
    data.current_time = ceil_timestamp_day(current_time);
    data.bytes_per_day = 1;   
    data.days_count = (data.current_time - data.start_time) / SECONDS_IN_DAY + 1;
    data.labels_count = 3;
    data.labels_buffer_count = data.bytes_per_day * 8;

    data.labels = malloc(sizeof(char*) * data.labels_buffer_count);
    for (int i = 0; i < data.labels_buffer_count; i++) {
        if (i < data.labels_count) data.labels[i] = calloc(MAX_LABEL_LENGTH, sizeof(char));
        else data.labels[i] = NULL;
    }

    data.data = malloc(sizeof(unsigned char*) * data.days_count);
    for(int i = 0; i < data.days_count; i++) {
        data.data[i] = malloc(sizeof(unsigned char) * data.bytes_per_day);
    }

    char str1[] = "limux";
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

void save_data(struct habit_data *data) {
    int fd = open("../data.bin", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        printf("error while opening the file");
        return;
    }

    write(fd, &data->start_time, sizeof(data->start_time));
    write(fd, &data->bytes_per_day, sizeof(data->bytes_per_day));
    write(fd, &data->labels_count, sizeof(data->labels_count));
    
    for (int i = 0; i < data->labels_buffer_count; i++) {
        if (i < data->labels_count) write(fd, data->labels[i], MAX_LABEL_LENGTH);
        else lseek(fd, MAX_LABEL_LENGTH, SEEK_CUR);
    }

    for (int i = 0; i < data->days_count; i++) {
        write(fd, data->data[i], data->bytes_per_day);
    }
    
    close(fd);
}

void free_allocated_habit_data(struct habit_data *data) {
    for (int i = 0; i < data->labels_buffer_count; i++) {
        if (data->labels[i] != NULL) free(data->labels[i]);
    }
    free(data->labels);

    for(int i = 0; i < data->days_count; i++) {
        free(data->data[i]);
    }
    free(data->data);
}
