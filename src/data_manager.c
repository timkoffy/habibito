#include "data_manager.h"

char *get_data_path() {
    const char *home = getenv("HOME");

    char *dir_path;
    asprintf(&dir_path, "%s/.habibito", home);

    mkdir(dir_path, 0755);

    char *full_path;
    asprintf(&full_path, "%s/data.bin", dir_path);
    free(dir_path);

    return full_path;
}

char *get_config_path() {
    const char *home = getenv("HOME");

    char *dir_path;
    asprintf(&dir_path, "%s/.config/habibito", home);

    mkdir(dir_path, 0755);

    char *full_path;
    asprintf(&full_path, "%s/config", dir_path);
    free(dir_path);

    return full_path;
}

int load_data(struct habit_data **dest, char *data_path, char *config_path, time_t current_time) {
    int fd = open(data_path, O_RDONLY);
    if (fd == -1) {
        return init_data(dest, current_time);
    }

    struct habit_data *data = malloc(sizeof(struct habit_data));

    read(fd, &(data->start_time), sizeof(data->start_time));
    read(fd, &(data->bytes_per_day), sizeof(data->bytes_per_day));
    read(fd, &(data->labels_count), sizeof(data->labels_count));
    data->labels_buffer_count = data->bytes_per_day * 8;
    data->current_time = ceil_timestamp_day(current_time);
    data->days_count = (data->current_time - data->start_time) / SECONDS_IN_DAY + 1;

    const int FILE_HEADER_SIZE = sizeof(data->start_time) + sizeof(data->bytes_per_day) + sizeof(data->labels_count);
    const int FILE_TOTAL_SIZE = lseek(fd, 0, SEEK_END);
    const int days_count_prev = (FILE_TOTAL_SIZE - FILE_HEADER_SIZE - data->labels_buffer_count * MAX_LABEL_LENGTH) / data->bytes_per_day;
    lseek(fd, FILE_HEADER_SIZE, SEEK_SET);

    if (days_count_prev > data->days_count) {
        print_error("previous days count larger than current days count");
        return 0;
    }

    data->labels = malloc(sizeof(char*) * data->labels_buffer_count);

    for (int label_idx = 0; label_idx < data->labels_buffer_count; label_idx++) {
        if (label_idx < data->labels_count) { 
            data->labels[label_idx] = malloc(MAX_LABEL_LENGTH); 
            read(fd, data->labels[label_idx], MAX_LABEL_LENGTH);
        } else {
            data->labels[label_idx] = NULL;
            lseek(fd, MAX_LABEL_LENGTH, SEEK_CUR);
        }
    }

    data->data = malloc(sizeof(uint8_t*) * data->days_count);

    for(int day_idx = 0; day_idx < data->days_count; day_idx++) {
        if (day_idx < days_count_prev) {
            data->data[day_idx] = malloc(data->bytes_per_day);
            read(fd, data->data[day_idx], data->bytes_per_day);
        } else {
            data->data[day_idx] = calloc(data->bytes_per_day, 1);
        }
    }

    close(fd);

    data->art_buffer = malloc(MAX_ART_WIDTH * 2);
    if (!parse_config_art(config_path, data->art_buffer)) {
        print_error("couldn't parse config");
    }

    *dest = data;

    return 1;
}

int init_data(struct habit_data **dest, time_t current_time) {
    struct habit_data *data = malloc(sizeof(struct habit_data));

    data->current_time = ceil_timestamp_day(current_time);
    data->start_time = data->current_time - SECONDS_IN_DAY * (DAYS_IN_WEEK - 1);
    data->days_count = DAYS_IN_WEEK;

    data->bytes_per_day = 1;
    data->labels_count = 0;
    data->labels_buffer_count = data->bytes_per_day * 8;
    
    data->labels = malloc(sizeof(char*) * data->labels_buffer_count);

    for (int label_idx = 0; label_idx < data->labels_buffer_count; label_idx++) {
        data->labels[label_idx] = NULL;
    }

    data->data = malloc(sizeof(uint8_t*) * data->days_count);

    for(int day_idx = 0; day_idx < data->days_count; day_idx++) {
        data->data[day_idx] = calloc(data->bytes_per_day, 1);
    }

    *dest = data;
    
    return 1;
}

int save_data(char *data_path, struct habit_data *data) {
    int fd = open(data_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        print_error("couldn't open the file");
        return 0;
    }

    write(fd, &(data->start_time), sizeof(data->start_time));
    write(fd, &(data->bytes_per_day), sizeof(data->bytes_per_day));
    write(fd, &(data->labels_count), sizeof(data->labels_count));
    
    for (int label_idx = 0; label_idx < data->labels_buffer_count; label_idx++) {
        if (label_idx < data->labels_count) write(fd, data->labels[label_idx], MAX_LABEL_LENGTH);
        else lseek(fd, MAX_LABEL_LENGTH, SEEK_CUR);
    }

    for (int day_idx = 0; day_idx < data->days_count; day_idx++) {
        write(fd, data->data[day_idx], data->bytes_per_day);
    }
    
    close(fd);

    return 1;
}

int init_paint_info(struct paint_info **dest) {
    struct paint_info *paint_info = malloc(sizeof(struct paint_info));

    *dest = paint_info;
    
    return 1;
}

void calculate_paint_info(struct paint_info *paint_info, struct habit_data *data) {
    struct winsize ws = get_screen_size(); 
    paint_info->rows = ws.ws_row;
    paint_info->cols = ws.ws_col;
    paint_info->total_height = HEADER_HEIGHT + data->labels_count;
    paint_info->max_label_width = max_label_width(data->labels, data->labels_count);
    paint_info->first_column_width = paint_info->max_label_width + 4;
    paint_info->cells_per_day = 3;
    paint_info->cells_per_week_border = 2;
    paint_info->cur_pos_day = data->days_count - 1;
    paint_info->cur_pos_habit = 0;
}


void free_allocated_structs(struct habit_data *data, struct paint_info *paint_info) {
    for (int label_idx = 0; label_idx < data->labels_buffer_count; label_idx++) {
        if (data->labels[label_idx] != NULL) free(data->labels[label_idx]);
    }
    free(data->labels);

    for(int day_idx = 0; day_idx < data->days_count; day_idx++) {
        free(data->data[day_idx]);
    }
    free(data->data);

    free(data->art_buffer);

    free(paint_info);
    free(data);
}

void toggle_habit_value(int day_idx, int habit_idx, struct habit_data *data) {
    int byte_idx = habit_idx / 8;
    int bit_idx = habit_idx % 8;
    change_bit(&data->data[day_idx][byte_idx], bit_idx);
}

int parse_config_art(char *dest, char *config_path) {
    /* the stupiest thing I've ever writen */

    int fd = open(config_path, O_RDONLY);
    if (fd == -1) {
//        print_error("couldn't open the config file");
        return 0;
    }

    lseek(fd, 0, SEEK_END);

    char cur_char;
    do {
        read(fd, &cur_char, 1);
        lseek(fd, -2, SEEK_CUR);
    } while(cur_char == '\n');

    int count_newline = 0;
    while(count_newline < 2) {
        read(fd, &cur_char, 1);
        if (cur_char == '\n') {
            count_newline++;
        }
        lseek(fd, -2, SEEK_CUR);
    }
    lseek(fd, 2, SEEK_CUR);
    count_newline = 0;
    
    int write_idx = 0;
    while(read(fd, &cur_char, 1)) {
        if (count_newline == 0 && write_idx >= MAX_ART_WIDTH) {
            while(cur_char != '\n') {
                read(fd, &cur_char, 1);
            }
        }
        if (count_newline == 1 && write_idx >= MAX_ART_WIDTH * 2) {
            break;
        }
        if (cur_char == '\n') {
            count_newline++;
            if (count_newline == 1) {
                while(write_idx < MAX_ART_WIDTH) {
                    dest[write_idx++] = ' ';
                }
                continue;
            }
            if (count_newline == 2) {
                while(write_idx < MAX_ART_WIDTH * 2) {
                    dest[write_idx++] = ' ';
                }
                break;
            }
        }
        dest[write_idx++] = cur_char;
    }

    if (write_idx != MAX_ART_WIDTH * 2) {
        print_error("while parsing art from config file to buffer");
        return 0;
    }
    
    return 1;
}
