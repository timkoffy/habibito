#pragma once 

#include <unistd.h>

#include "../src/utils.c"

volatile short running;

struct winsize get_screen_size();

void setup_terminal();
