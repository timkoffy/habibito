#pragma once 

#include "habit_data.h"
#include "painter.h"

void handle_move_up(struct habit_data *data, struct paint_info *paint_info);
void handle_move_down(struct habit_data *data, struct paint_info *paint_info);
void handle_move_right(struct habit_data *data, struct paint_info *paint_info);
void handle_move_left(struct habit_data *data, struct paint_info *paint_info);

void handle_toggle_habit_value(struct habit_data *data, struct paint_info *paint_info);

void handle_add_label(bool *input_mode, 
                      struct habit_data *data, struct paint_info *paint_info);

void handle_input(char input, 
                  bool *input_mode, bool *is_any_letter, int *input_len, char *input_buf, 
                  struct habit_data *data, struct paint_info *paint_info);

void handle_info_mode(bool *info_mode, 
                      struct habit_data *data, struct paint_info *paint_info);
