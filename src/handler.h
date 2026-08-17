#pragma once 

#include "habit_data.h"
#include "painter.h"
#include "utils.h"

void move_up_handler(struct habit_data *data, struct paint_info *paint_info);
void move_down_handler(struct habit_data *data, struct paint_info *paint_info);
void move_right_handler(struct habit_data *data, struct paint_info *paint_info);
void move_left_handler(struct habit_data *data, struct paint_info *paint_info);

void toggle_habit_value_handler(struct habit_data *data, struct paint_info *paint_info);

void add_label_handler(bool *input_mode, 
                      struct habit_data *data, struct paint_info *paint_info);

void input_handler(char input, 
                  bool *input_mode, bool *is_any_letter, int *input_len, char *input_buf, 
                  struct habit_data *data, struct paint_info *paint_info);

void info_mode_handler(bool *info_mode, 
                      struct habit_data *data, struct paint_info *paint_info);
