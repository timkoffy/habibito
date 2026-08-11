#include "handler.h"

void handle_move_up(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_habit > 0) {
        paint_info->cur_pos_habit--;
        draw_calendar(data, paint_info);
    }
}

void handle_move_down(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_habit < data->labels_count - 1) {
        paint_info->cur_pos_habit++;
        draw_calendar(data, paint_info);
    }
}

void handle_move_right(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_day + 1 < data->days_count) {
        paint_info->cur_pos_day++;
        draw_calendar(data, paint_info);
    } 
}

void handle_move_left(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_day > 0) {
        paint_info->cur_pos_day--;
        draw_calendar(data, paint_info);
    }
}


void handle_toggle_habit(struct habit_data *data, struct paint_info *paint_info) {
    toggle_habit(paint_info->cur_pos_day, paint_info->cur_pos_habit, data);
    draw_calendar(data, paint_info);
}


void handle_add_label(bool *input_mode, struct habit_data *data, struct paint_info *paint_info) {
    if (data->labels_count < 8) {
        *input_mode = true;
        printf("\e[?25h"); // show cursor
        move_cursor(0, HEADER_HEIGHT + data->labels_count + 1); 
        printf("\e[2K"); // erase from cursor to end of line
        printf("%d ", data->labels_count);
        fflush(stdout);
    }
}


void handle_info_mode(bool *info_mode, struct habit_data *data, struct paint_info *paint_info) {
    *info_mode = !(*info_mode); 
    if (*info_mode) draw_info_screen(data, paint_info);
    else {
        draw_screen(data, paint_info);
        draw_calendar(data, paint_info);
    }
}
