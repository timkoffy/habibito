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
    printf("\e[H");
    fflush(stdout);
    for (int i = 0; i < paint_info->rows; i++) {
        if (i >= paint_info->total_height && i < paint_info->rows) {
            printf("\n");
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
        printf("pdest-> 'a' to add habit");
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

    // printf("info: capacity = %d, n_days = %d, n_weeks = %d\n", capacity, n_days, n_weeks_ceiled);

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
                // printf(" |");
            }
        }


        // data visual




        if (no_data) {
            printf("\e[39m");
        }    
        
        timestamp_tmp += SECONDS_IN_DAY; 
    }
    fflush(stdout);
}
