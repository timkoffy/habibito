#pragma once 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>   
#include <fcntl.h>  
#include <unistd.h>

#include "habit_data.h"
#include "config.h"

struct habit_data load_data(const char *path);

struct habit_data load_mock_data();

void save_data(struct habit_data *data);

void free_allocated_habit_data(struct habit_data *data);
