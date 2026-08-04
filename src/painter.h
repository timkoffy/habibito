#pragma once

#include <stdio.h>

#include "data_manager.h"
#include "config.h"
#include "utils.h"

struct paint_info {
    unsigned short rows;
    unsigned short cols;
    unsigned short total_height;
    unsigned short max_label_width;
    unsigned short first_column_width;
    unsigned short cells_per_day;
    unsigned short cells_per_week_border;
    unsigned int cur_pos_day;
    unsigned short cur_pos_habit;
};

void calculate_paint_info(struct paint_info *dest, unsigned short rows, unsigned short cols, struct habit_data *data);
void draw_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_calendar(struct habit_data *data, struct paint_info *paint_info);
void draw_info_screen(struct habit_data *data, struct paint_info *paint_info);
