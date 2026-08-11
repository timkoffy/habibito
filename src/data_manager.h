#pragma once 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>   
#include <fcntl.h>  
#include <unistd.h>

#include "habit_data.h"
#include "config.h"
#include "utils.h"

#define DATA_PATH "../data.bin" 
#define CONFIG_PATH "../config"

struct habit_data load_data(time_t current_time);
struct habit_data load_mock_data(time_t current_time);
void save_data(struct habit_data *data);
void free_allocated_data(struct habit_data *data);
struct habit_data init_data(time_t current_time);

void toggle_habit_value(int day_idx, int habit_idx, struct habit_data *data);

int parse_config_art(char *dest);
