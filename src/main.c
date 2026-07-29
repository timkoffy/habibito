#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <string.h>

#include "../include/painter.h"
#include "../include/utils.h"

void run() {
    setup_terminal();

    struct winsize ws = get_screen_size();

    draw_screen(ws.ws_col, ws.ws_row);
    fflush(stdout);
    
    while(running) {

    }
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
