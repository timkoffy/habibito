#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"
#include "handler.h"

void run() {
    setup_terminal();

    char *data_path = get_data_path();
    char *config_path = get_config_path();

    time_t current_time = time(NULL);
    struct habit_data data = load_data(data_path, config_path, current_time);

    struct winsize ws = get_screen_size();
    struct paint_info paint_info;
    calculate_paint_info(&paint_info, ws.ws_row, ws.ws_col, &data);

    draw_screen(&data, &paint_info);
    draw_calendar(&data, &paint_info);

    bool input_mode = false;
    bool info_mode = false;

    int input_len = 0;
    char input;
    char input_buf[MAX_LABEL_LENGTH]; 
    bool is_any_letter = false;

    while (running) {
        int read_count = read(STDIN_FILENO, &input, 1);
        if (read_count > 0) {
            if (input_mode) {
                input_handler(input, &input_mode, &is_any_letter, &input_len, input_buf, &data, &paint_info);
                continue;
            } 

            switch (input) {
                case 'k': move_up_handler(&data, &paint_info); break; 
                case 'j': move_down_handler(&data, &paint_info); break;
                case 'l': move_right_handler(&data, &paint_info); break;
                case 'h': move_left_handler(&data, &paint_info); break;
                case 'a': add_label_handler(&input_mode, &data, &paint_info); break;
                case '\n': case 'i': toggle_habit_value_handler(&data, &paint_info); break;
                case 'q': interrupt_handler(SIGINT); break;
                case 's': info_mode_handler(&info_mode, &data, &paint_info); break; // dev util to show values of all application variables
            }
        }
    }

    save_data(data_path, &data);
    free_allocated_data(&data);
    free(data_path);
    free(config_path);
}

int main(int argc, char *argv[]) {
    run();
    return 0;
}
