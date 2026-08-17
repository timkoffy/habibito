#pragma once

#include <stdio.h>

#include "data_manager.h"
#include "config.h"
#include "utils.h"

void draw_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_calendar(struct habit_data *data, struct paint_info *paint_info);
void draw_info_screen(struct habit_data *data, struct paint_info *paint_info);
void draw_art(struct habit_data *data, struct paint_info *paint_info);
