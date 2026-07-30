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
};

struct paint_info calculate_paint_info(unsigned short rows, unsigned short cols, struct habit_data *data);
void draw_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_calendar(time_t timestamp_r, struct habit_data *data, struct paint_info *paint_info);

