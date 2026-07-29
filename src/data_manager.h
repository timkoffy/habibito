#pragma once 

#include "../include/habit_data.h"

struct habit_data load_data(const char *path);

struct habit_data load_mock_data();

void save_data(struct habit_data);


