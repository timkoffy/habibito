#include <stdio.h>
#include <time.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"

void run() {
    setup_terminal();
    struct winsize ws = get_screen_size();
    time_t current_time = time(NULL);
    struct habit_data data = load_mock_data(current_time);
    struct paint_info paint_info = calculate_paint_info(ws.ws_row, ws.ws_col, &data);

    draw_screen(&data, &paint_info);
    draw_calendar(3, &data, &paint_info);
    fflush(stdout);
 
    while (running) {
    }

    save_data(&data);
    free_allocated_habit_data(&data);
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
