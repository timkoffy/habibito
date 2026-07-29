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
 
    char buf[10];
    while (running) {
        int count = read(0, &buf, 10);
        if(count > 0) {
            char ch = count + '0';
            // write(STDOUT_FILENO, "\e[5;4H", 6);
            write(STDOUT_FILENO, &ch, 1);
            update_screen(buf);
        }
    }
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
