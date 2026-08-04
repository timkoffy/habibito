#include "painter.h"

void calculate_paint_info(struct paint_info *dest, unsigned short rows, unsigned short cols, struct habit_data *data) {
    dest->rows = rows;
    dest->cols = cols;
    dest->total_height = HEADER_HEIGHT + data->labels_count;
    dest->max_label_width = max_label_width(data->labels, data->labels_count);
    dest->first_column_width = dest->max_label_width + 4;
    dest->cells_per_day = 3;
    dest->cells_per_week_border = 2;
    dest->cur_pos_day = data->days_count - 1;
    dest->cur_pos_habit = 0;
}

void draw_screen(struct habit_data *data, struct paint_info *paint_info) {
    printf("\e[%d;999H\e[1J\e[H", paint_info->rows - 1);
    fflush(stdout);
    for (int i = 0; i < paint_info->rows; i++) {
        if (i >= paint_info->total_height && i < paint_info->rows) {
            continue;
        }

        for (int j = 0; j < paint_info->cols; j++) {
            if (i == HEADER_HEIGHT - 1) { 
                printf("-");
                continue;
            }
            if (j == 0 && i >= HEADER_HEIGHT) {
                int label_idx = i - HEADER_HEIGHT;
                printf("%d", label_idx);
                j += printf(" %s", data->labels[label_idx]);
                continue;
            }
            if (j == paint_info->max_label_width + 3) {
                printf("|");
                continue;
            }
            printf(" ");
        }
    }
    if (data->labels_count == 0) {
        move_cursor(0, paint_info->total_height + 1);
        printf("press 'a' to add habit...");
    }

    fflush(stdout);
}

void draw_calendar(struct habit_data *data, struct paint_info *paint_info) {
    int capacity = paint_info->cols - paint_info->first_column_width - 2;
    int n_weeks_ceiled = (capacity + (DAYS_IN_WEEK * paint_info->cells_per_day - 1)) / (DAYS_IN_WEEK * paint_info->cells_per_day);
    int n_days = (capacity - (n_weeks_ceiled * 2)) / 3;

    time_t timestamp_cur = data->start_time + paint_info->cur_pos_day * SECONDS_IN_DAY;
    time_t timestamp_right = timestamp_cur + (n_days / 2) * SECONDS_IN_DAY;
    time_t timestamp_tmp = timestamp_right - n_days * SECONDS_IN_DAY;
    int cur_day_tmp = paint_info->cur_pos_day + n_days / 2 - n_days;

    // printf("info: capacity = %d, n_days = %d, n_weeks = %d, cur_day_tmp = %d\n", capacity, n_days, n_weeks_ceiled, cur_day_tmp);
    for (int i = 0; i < data->labels_count; i++) {
        printf("\e[%d;%dH\e[0K", i + HEADER_HEIGHT + 1, paint_info->first_column_width + 1);
    }
    fflush(stdout);

    for (int i = paint_info->first_column_width + 2; i < paint_info->cols; i += paint_info->cells_per_day) {
        struct tm *time_info;
        time_info = localtime(&timestamp_tmp);
        int weekday = time_info->tm_wday;
        int monthday = time_info->tm_mday;
        
        int no_data = 0;
        if (timestamp_tmp < data->start_time || timestamp_tmp > (data->start_time + (data->days_count - 1) * SECONDS_IN_DAY)) {
            no_data = 1;
        }    

        if (no_data) {
            printf("\e[90m");
        }

        // position visual
        if (timestamp_tmp == timestamp_cur) {
            for (int j = 0; j < data->labels_count; j++) {
                move_cursor(i - 2, j + HEADER_HEIGHT + 1);
                if (paint_info->cur_pos_habit == j) {
                    printf("  +    ");
                    continue;
                }
                printf("  .    ");
            }
        }
        
        // data visual
        if (!no_data && cur_day_tmp >= 0 && cur_day_tmp < data->days_count) {
            for (int label_idx = 0; label_idx < data->labels_count; label_idx++) {
                move_cursor(i, label_idx + HEADER_HEIGHT + 1);
                if (is_bit_true(&data->data[cur_day_tmp][label_idx / 8], label_idx % 8)) {
                    printf("* ");
                } else {
                    printf("- ");
                }
            }
        }

        // calendar visual
        move_cursor(i, 1);
        printf("%02d ", monthday);
        move_cursor(i, 2);
        printf("%c  ", get_char_from_weekday(weekday));

        if (weekday == 6 && i + 4 < paint_info->cols) {
            move_cursor(i + 3, 1);
            printf("| ");
            move_cursor(i + 3, 2);
            printf("| ");
            i += paint_info->cells_per_week_border;
            
            for (int j = 0; j < data->labels_count; j++) {
                move_cursor(i, j + HEADER_HEIGHT + 1);
                printf(" |");
            }
        }

        if (no_data) {
            printf("\e[39m");
        }    
        
        timestamp_tmp += SECONDS_IN_DAY; 
        cur_day_tmp++;
    }
    fflush(stdout);
}

void draw_info_screen(struct habit_data *data, struct paint_info *paint_info) {
    printf("\e[2J\e[H");
    printf("some statistics\n\n");
    printf("===== HABIT DATA STRUCT INFO =====\n");
    
    char start_str_time[20];
    char cur_str_time[20];

    strftime(start_str_time, 20, "%D", localtime(&data->start_time));
    strftime(cur_str_time, 20, "%D", localtime(&data->current_time));

    printf("start_time:            %s\n", start_str_time);
    printf("current_time:          %s\n", cur_str_time);
    printf("bytes_per_day:         %d\n", data->bytes_per_day);
    printf("days_count:            %d\n", data->days_count);
    printf("labels_count:          %d\n", data->labels_count);
    printf("labels_buffer_count:   %d\n", data->labels_buffer_count);

    printf("\n===== PAINT INFO STRUCT INFO =====\n");
    
    printf("rows:                  %d\n", paint_info->rows);
    printf("cols:                  %d\n", paint_info->cols);
    printf("total_height           %d\n", paint_info->total_height);
    printf("max_label_width:       %d\n", paint_info->max_label_width);
    printf("first_column_width:    %d\n", paint_info->first_column_width);
    printf("cur_pos_day:           %d\n", paint_info->cur_pos_day);
    printf("cur_pos_habit:         %d\n", paint_info->cur_pos_habit);

    // printf("\nLAST CURSOR POSITION: row = %d, col = %d\n");

    fflush(stdout);
}
