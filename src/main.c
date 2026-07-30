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

    char input;
    int cur_pos = 6;

    draw_screen(&data, &paint_info);
    draw_calendar(cur_pos, &data, &paint_info);
 
    while (running) {
        int count = read(STDIN_FILENO, &input, 1);
        if (count > 0) {
            switch (input) {
                case 'h':
                    draw_calendar(--cur_pos, &data, &paint_info);
                    break;
                case 'l':
                    draw_calendar(++cur_pos, &data, &paint_info);
                    break;
                case 'q':
                    interrupt_handler(SIGINT);
                    break;
            }
        }
    }

    save_data(&data);
    free_allocated_habit_data(&data);
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
