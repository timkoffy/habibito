#pragma once

#include "../include/data_manager.h"

#define MIN_FIRST_COL_WIDTH = 17;
#define HEADER_HEIGHT = 3;

void draw_screen(unsigned short cols, unsigned short rows, struct habit_data *data);
void update_screen(char *buf);

