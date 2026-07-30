#include "painter.h"

struct paint_info calculate_paint_info(unsigned short rows, unsigned short cols, struct habit_data *data) {
    struct paint_info res;
    res.rows = rows;
    res.cols = cols;
    res.total_height = HEADER_HEIGHT + data->labels_count;
    res.max_label_width = max_label_width(data->labels, data->labels_count);
    res.first_column_width = res.max_label_width + 5;

    return res;
}

void draw_screen(struct habit_data *data, struct paint_info *paint_info) {
    for (int i = 0; i < paint_info->rows; i++) {
        if (i >= paint_info->total_height) {
            break;
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
}

void draw_calendar(int day_idx_right, struct habit_data *data, struct paint_info *paint_info) {
    time_t timestamp_day_right = data->start_time + day_idx_right * SECONDS_IN_DAY;
    time_t timestamp_tmp = timestamp_day_right;
    for (int i = paint_info->cols - 1, count = 0; i > paint_info->first_column_width; i -= 3, count++) {
        timestamp_tmp -= SECONDS_IN_DAY;
        int weekday_tmp = get_weekday_from_timestamp(&timestamp_tmp);
        printf("weekday at %d cell: %d\n", i, weekday_tmp);
    }
}
