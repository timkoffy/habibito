#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <string.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"
#include "habit_data.h"

void run() {
    setup_terminal();

    struct winsize ws = get_screen_size();

    struct habit_data data = load_mock_data();

    draw_screen(ws.ws_col, ws.ws_row, &data);
    fflush(stdout);
 
    while (running) {
    }
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
