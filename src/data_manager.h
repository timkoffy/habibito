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

char *get_data_path();
char *get_config_path();

struct habit_data load_data(char *data_path, char *config_path, time_t current_time);
struct habit_data init_data(time_t current_time);
void save_data(char *data_path, struct habit_data *data);
void free_allocated_data(struct habit_data *data);

void toggle_habit_value(int day_idx, int habit_idx, struct habit_data *data);

int parse_config_art(char *config_path, char *dest);
