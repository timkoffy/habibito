#pragma once 

#include <unistd.h>

volatile short running;

struct winsize get_screen_size();

void setup_terminal();
