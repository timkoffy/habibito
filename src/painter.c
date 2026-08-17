#include "painter.h"

void draw_screen(struct habit_data *data, struct paint_info *paint_info) {
    printf("\e[%d;999H\e[1J\e[H", paint_info->rows - 1);
    fflush(stdout);
    for (int row_idx = 0; row_idx < paint_info->rows; row_idx++) {
        if (row_idx >= paint_info->total_height && row_idx < paint_info->rows) {
            continue;
        }

        for (int col_idx = 0; col_idx < paint_info->cols; col_idx++) {
            if (row_idx == HEADER_HEIGHT - 1) { 
                if (col_idx == paint_info->first_column_width - 1) {
                    printf("+\n");
                    break;
                }
                printf("-");
                continue;
            }
            if (col_idx == 0 && row_idx >= HEADER_HEIGHT) {
                int label_idx = row_idx - HEADER_HEIGHT;
                printf("%d", label_idx);
                col_idx += printf(" %s", data->labels[label_idx]);
                continue;
            }
            if (col_idx == paint_info->max_label_width + 3) {
                printf("|");
                continue;
            }
            printf(" ");
        }
    }
    if (data->labels_count == 0) {
        move_cursor(paint_info->total_height + 1, 0);
        printf("press 'a' to add habit...");
    }

    draw_art(data, paint_info);
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

    // clear prev state of calendar
    for (int label_idx = 0; label_idx < data->labels_count; label_idx++) {
        printf("\e[%d;%dH\e[0K", label_idx + HEADER_HEIGHT + 1, paint_info->first_column_width + 1);
    }
    fflush(stdout);

    for (int col_idx = paint_info->first_column_width + 2; col_idx < paint_info->cols - 1; col_idx += paint_info->cells_per_day) {
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

        // data visual
        if (!no_data && cur_day_tmp >= 0 && cur_day_tmp < data->days_count) {
            for (int label_idx = 0; label_idx < data->labels_count; label_idx++) {
                move_cursor(label_idx + HEADER_HEIGHT + 1, col_idx - 1);
                if (paint_info->cur_pos_habit == label_idx && paint_info->cur_pos_day == cur_day_tmp) {
                    printf("\e[90m[\e[39m");
                } else printf(" ");
                
                if (is_bit_true(&data->data[cur_day_tmp][label_idx / 8], label_idx % 8)) {
                    printf("*");
                } else {
                    printf("\e[90m-\e[39m");
                }

                if (paint_info->cur_pos_habit == label_idx && paint_info->cur_pos_day == cur_day_tmp) {
                    printf("\e[90m] \e[39m");
                } else printf("  ");
            }
        }

        // calendar visual
        move_cursor(1, col_idx);
        printf("%02d ", monthday);
        move_cursor(2, col_idx);
        printf("%c  ", get_char_from_weekday(weekday));
        move_cursor(3, col_idx); 
        printf("---");

        if (weekday == 6 && col_idx + 4 < paint_info->cols) {
            move_cursor(1, col_idx + paint_info->cells_per_week_border);
            printf(" | ");
            move_cursor(2, col_idx + paint_info->cells_per_week_border);
            printf(" | ");
            move_cursor(3, col_idx + paint_info->cells_per_week_border);
            printf("-+-");
            col_idx += paint_info->cells_per_week_border;
            
            for (int label_idx = 0; label_idx < data->labels_count; label_idx++) {
                move_cursor(label_idx + HEADER_HEIGHT + 1, col_idx);
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
    
    printf("\n=====  ART FROM CONFIG FILE  =====\n");
    write(STDOUT_FILENO, data->art_buffer, MAX_ART_WIDTH);
    printf("||||||| just to show ||||||||||||\n");
    write(STDOUT_FILENO, data->art_buffer + MAX_ART_WIDTH, MAX_ART_WIDTH);
    printf("||||||||||| border of art |||||||\n");


    fflush(stdout);
}

void draw_art(struct habit_data *data, struct paint_info *paint_info) {
    move_cursor(1, 1);
    write(STDOUT_FILENO, data->art_buffer, paint_info->first_column_width - 1);
    move_cursor(2, 1);
    write(STDOUT_FILENO, data->art_buffer + MAX_ART_WIDTH, paint_info->first_column_width - 1);
}
