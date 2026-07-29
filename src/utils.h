#pragma once 

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "config.h"

extern volatile short running;

struct winsize get_screen_size();

void setup_terminal();

unsigned short max_label_width(char **labels, unsigned short labels_count);
