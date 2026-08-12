#include "handler.h"

void move_up_handler(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_habit > 0) {
        paint_info->cur_pos_habit--;
        draw_calendar(data, paint_info);
    }
}

void move_down_handler(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_habit < data->labels_count - 1) {
        paint_info->cur_pos_habit++;
        draw_calendar(data, paint_info);
    }
}

void move_right_handler(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_day + 1 < data->days_count) {
        paint_info->cur_pos_day++;
        draw_calendar(data, paint_info);
    } 
}

void move_left_handler(struct habit_data *data, struct paint_info *paint_info) {
    if (paint_info->cur_pos_day > 0) {
        paint_info->cur_pos_day--;
        draw_calendar(data, paint_info);
    }
}


void toggle_habit_value_handler(struct habit_data *data, struct paint_info *paint_info) {
    toggle_habit_value(paint_info->cur_pos_day, paint_info->cur_pos_habit, data);
    draw_calendar(data, paint_info);
}


void add_label_handler(bool *input_mode,
                      struct habit_data *data, struct paint_info *paint_info) {

    if (data->labels_count < 8) {
        *input_mode = true;
        printf("\e[?25h"); // show cursor
        move_cursor(0, HEADER_HEIGHT + data->labels_count + 1); 
        printf("\e[2K"); // erase from cursor to end of line
        printf("%d ", data->labels_count);
        fflush(stdout);
    }
}


void input_handler(char input, 
                  bool *input_mode, bool *is_any_letter, int *input_len, char *input_buf, 
                  struct habit_data *data, struct paint_info *paint_info) {

    if (input == ' ' && !(*is_any_letter)) return;

    *is_any_letter = true;
    
    if (*input_len == MAX_LABEL_LENGTH || input == '\n') { // apply label
        // reset all state variables
        *input_mode = false;
        *input_len = 0;
        *is_any_letter = false;

        printf("\e[?25l");
        if (input == '\n') printf("\e[1A"); 
        printf("\e[2K");
        fflush(stdout);

        // allocate memory for new label
        data->labels[data->labels_count] = calloc(MAX_LABEL_LENGTH, 1);
        memcpy(data->labels[data->labels_count], input_buf, MAX_LABEL_LENGTH);
        memset(input_buf, 0, MAX_LABEL_LENGTH);
        data->labels_count++;

        calculate_paint_info(paint_info, paint_info->rows, paint_info->cols, data);
        draw_screen(data, paint_info);
        draw_calendar(data, paint_info);

        return;
    }
    
    write(STDOUT_FILENO, &input, 1);
    memcpy((void*)input_buf + (*input_len), &input, 1);
    
    (*input_len)++;
}


void info_mode_handler(bool *info_mode, 
                      struct habit_data *data, struct paint_info *paint_info) {

    *info_mode = !(*info_mode); 
    if (*info_mode) draw_info_screen(data, paint_info);
    else {
        draw_screen(data, paint_info);
        draw_calendar(data, paint_info);
    }
}
