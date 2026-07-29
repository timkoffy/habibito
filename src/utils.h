#pragma once 

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

extern volatile short running;

struct winsize get_screen_size();

void setup_terminal();
