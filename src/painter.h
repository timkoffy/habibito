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
};

struct paint_info calculate_paint_info(unsigned short rows, unsigned short cols, struct habit_data *data);
void draw_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_calendar(int day_idx_right, struct habit_data *data, struct paint_info *paint_info);

