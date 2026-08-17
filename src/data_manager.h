#pragma once 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>   
#include <fcntl.h>  
#include <unistd.h>
#include <sys/stat.h>

#include "habit_data.h"
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

char *get_data_path();
char *get_config_path();

int load_data(struct habit_data **dest, char *data_path, char *config_path, time_t current_time);
int init_data(struct habit_data **dest, time_t current_time);
int save_data(char *data_path, struct habit_data *data);

int init_paint_info(struct paint_info **dest);
void calculate_paint_info(struct paint_info *paint_info, struct habit_data *data);

void free_allocated_structs(struct habit_data *data, struct paint_info *paint_info);

void toggle_habit_value(int day_idx, int habit_idx, struct habit_data *data);

int parse_config_art(char *dest, char *config_path);
