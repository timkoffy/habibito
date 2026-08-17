#pragma once

#include <stdio.h>

#include "data_manager.h"
#include "config.h"
#include "utils.h"

struct paint_info {
    int rows;
    int cols;
    int total_height;
    int max_label_width;
    int first_column_width;
    int cells_per_day;
    int cells_per_week_border;
    int cur_pos_day;
    int cur_pos_habit;
};

void calculate_paint_info(struct paint_info *dest, struct habit_data *data);
void draw_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_calendar(struct habit_data *data, struct paint_info *paint_info);
void draw_info_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_art(struct habit_data *data, struct paint_info *paint_info);
