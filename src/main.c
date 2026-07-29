#include <stdio.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"

void run() {
    setup_terminal();
    struct winsize ws = get_screen_size();

    struct habit_data data = load_mock_data();

    draw_screen(ws.ws_col, ws.ws_row, &data);
    fflush(stdout);
 
    while (running) {
    }

    free_allocated_habit_data(data);
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
