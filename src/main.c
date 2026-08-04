#include <stdio.h>
#include <time.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"

void run() {
    setup_terminal();
    struct winsize ws = get_screen_size();
    time_t current_time = time(NULL);
    struct habit_data data = load_data(current_time);
    struct paint_info paint_info = calculate_paint_info(ws.ws_row, ws.ws_col, &data);

//    printf("%s, %d, %d, %d\n", asctime(localtime(&data.current_time)), data.bytes_per_day, data.labels_count, data.days_count);

    draw_screen(&data, &paint_info);
    draw_calendar(&data, &paint_info);
    
    short input_mode = 0;
    short label_len_tmp = 0;
    short is_any_letter = 0;
    char input;
    while (running) {
        int count = read(STDIN_FILENO, &input, 1);
        if (count > 0) {
            if (input_mode) {
                if (input == ' ' && is_any_letter == 0) continue;

                write(STDOUT_FILENO, &input, 1);
                
                is_any_letter = 1;
                label_len_tmp++;
                
                if (label_len_tmp == MAX_LABEL_LENGTH || input == '\n') {
                    is_any_letter = 0;
                    label_len_tmp = 0;
                    input_mode = 0;

                    printf("\e[?25l");
                    if (input == '\n') printf("\e[1A"); 
                    printf("\e[2K");
                    fflush(stdout);
                }

                continue;
            } 

            switch (input) {
                case 'h':
                    if (paint_info.cur_pos_day > 0) {
                        paint_info.cur_pos_day--;
                        draw_calendar(&data, &paint_info);
                    }
                    break;
                case 'l':
                    if (paint_info.cur_pos_day + 1 < data.days_count) {
                        paint_info.cur_pos_day++;
                        draw_calendar(&data, &paint_info);
                    } 
                    break;
                case 'k':
                    if (paint_info.cur_pos_habit > 0) {
                        paint_info.cur_pos_habit--;
                        draw_calendar(&data, &paint_info);
                    }
                    break;
                case 'j':
                    if (paint_info.cur_pos_habit < data.labels_count - 1) {
                        paint_info.cur_pos_habit++;
                        draw_calendar(&data, &paint_info);
                    }
                    break;
                case 'a':
                    if (data.labels_count < 8) {
                        input_mode = 1;
                        printf("\e[?25h");
                        move_cursor(0, HEADER_HEIGHT + data.labels_count + 1); 
                        printf("\e[2K");
                        printf("%d ", data.labels_count);
                        fflush(stdout);
                    }
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
