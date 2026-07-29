#pragma once

#include <stdio.h>

#include "data_manager.h"
#include "config.h"
#include "utils.h"

void draw_screen(unsigned short cols, unsigned short rows, struct habit_data *data);
void update_screen(char *buf);

