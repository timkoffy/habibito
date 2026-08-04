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

struct habit_data load_data(time_t current_time) {
    int fd = open(DATA_PATH, O_RDONLY);
    if (fd == -1) {
        return init_data(current_time);
    }

    struct habit_data data;
    read(fd, &data.start_time, sizeof(data.start_time));
    read(fd, &data.bytes_per_day, sizeof(data.bytes_per_day));
    read(fd, &data.labels_count, sizeof(data.labels_count));
    data.labels_buffer_count = data.bytes_per_day * 8;
    data.current_time = ceil_timestamp_day(current_time);
    data.days_count = (data.current_time - data.start_time) / SECONDS_IN_DAY + 1;

    const int FILE_HEADER_SIZE = sizeof(data.start_time) + sizeof(data.bytes_per_day) + sizeof(data.labels_count);
    const int FILE_TOTAL_SIZE = lseek(fd, 0, SEEK_END);
    const int days_count_prev = (FILE_TOTAL_SIZE - FILE_HEADER_SIZE - data.labels_buffer_count * MAX_LABEL_LENGTH) / data.bytes_per_day;
    lseek(fd, FILE_HEADER_SIZE, SEEK_SET);

    if (days_count_prev < data.days_count) {
        printf("error: previous days count larger than current days count\n");
        return data;
    }

    // printf("\nstart_time = %d | current_time = %d | bytes_per_day = %d | labels_count = %d", data.start_time, data.current_time, data.bytes_per_day, data.labels_count);

    data.labels = malloc(sizeof(char*) * data.labels_buffer_count);
    for (int i = 0; i < data.labels_buffer_count; i++) {
        if (i < data.labels_count) {
            data.labels[i] = malloc(MAX_LABEL_LENGTH);
            read(fd, data.labels[i], MAX_LABEL_LENGTH);
        } else {
            data.labels[i] = NULL;
            lseek(fd, MAX_LABEL_LENGTH, SEEK_CUR);
        }
    }

    data.data = malloc(sizeof(char*) * data.days_count);
    for(int i = 0; i < data.days_count; i++) {
        if (i < days_count_prev) {
            data.data[i] = malloc(data.bytes_per_day);
            read(fd, data.data[i], data.bytes_per_day);
        } else {
            data.data[i] = calloc(1, data.bytes_per_day);
        }
    }

    close(fd);
    return data;
}

struct habit_data init_data(time_t current_time) {
    struct habit_data data;
    data.current_time = ceil_timestamp_day(current_time);
    data.start_time = data.current_time - SECONDS_IN_DAY * (DAYS_IN_WEEK - 1);
    data.days_count = DAYS_IN_WEEK;

    data.bytes_per_day = 1;
    data.labels_count = 0;
    data.labels_buffer_count = data.bytes_per_day * 8;
    
    data.labels = malloc(sizeof(char*) * data.labels_buffer_count);
    for (int i = 0; i < data.labels_buffer_count; i++) {
        data.labels[i] = NULL;
    }

    data.data = malloc(sizeof(char*) * data.days_count);
    for(int i = 0; i < data.days_count; i++) {
        data.data[i] = calloc(1, data.bytes_per_day);
    }
    
    /*
    char start_str_time[20];
    char cur_str_time[20];

    strftime(start_str_time, 20, "%D", localtime(&data.start_time));
    strftime(cur_str_time, 20, "%D", localtime(&data.current_time));

    printf("[init_data] days_count: %d | start_time: %s | current_time: %s\n", data.days_count, start_str_time, cur_str_time);
    */

    return data;
}

/*
[init_data] days_count: 7 | start_time: 07/28/26 | current_time: 08/04/26

28 
29
30 
31 
1 
2 
3 
4 
*/

void save_data(struct habit_data *data) {
    int fd = open(DATA_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("error while opening the file\n");
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

void free_allocated_data(struct habit_data *data) {
    for (int i = 0; i < data->labels_buffer_count; i++) {
        if (data->labels[i] != NULL) free(data->labels[i]);
    }
    free(data->labels);

    for(int i = 0; i < data->days_count; i++) {
        free(data->data[i]);
    }
    free(data->data);
}
