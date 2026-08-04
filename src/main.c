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
    struct paint_info paint_info;

//    printf("%s, %d, %d, %d\n", asctime(localtime(&data.current_time)), data.bytes_per_day, data.labels_count, data.days_count);

    calculate_paint_info(&paint_info, ws.ws_row, ws.ws_col, &data);
    draw_screen(&data, &paint_info);
    draw_calendar(&data, &paint_info);
   
    short input_mode = 0;
    short input_len = 0;
    short is_any_letter = 0;
    char input;
    char input_buf[MAX_LABEL_LENGTH]; 

    short is_info_screen = 0;

    while (running) {
        int count = read(STDIN_FILENO, &input, 1);
        if (count > 0) {
            if (input_mode) {
                if (input == ' ' && is_any_letter == 0) continue;

                is_any_letter = 1;
                
                if (input_len + 1 == MAX_LABEL_LENGTH || input == '\n') {
                    is_any_letter = 0;
                    input_len = 0;
                    input_mode = 0;

                    printf("\e[?25l");
                    if (input == '\n') printf("\e[1A"); 
                    printf("\e[2K");
                    fflush(stdout);
    
                    // allocate memory for new label
                    data.labels[data.labels_count] = calloc(MAX_LABEL_LENGTH, 1);
                    memcpy(data.labels[data.labels_count], input_buf, MAX_LABEL_LENGTH);
                    memset(input_buf, 0, MAX_LABEL_LENGTH);
                    data.labels_count++;

                    calculate_paint_info(&paint_info, ws.ws_row, ws.ws_col, &data);
                    draw_screen(&data, &paint_info);
                    draw_calendar(&data, &paint_info);

                    continue;
                }
                
                write(STDOUT_FILENO, &input, 1);
                memcpy((void*)input_buf + input_len, &input, 1);
                
                input_len++;

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
                        printf("\e[?25h"); // show cursor
                        move_cursor(0, HEADER_HEIGHT + data.labels_count + 1); 
                        printf("\e[2K"); // erase from cursor to end of line
                        printf("%d ", data.labels_count);
                        fflush(stdout);
                    }
                    break;
                case '\n':
                    toggle_habit(paint_info.cur_pos_day, paint_info.cur_pos_habit, &data);
                    draw_calendar(&data, &paint_info);
                    break;
                case 'q':
                    interrupt_handler(SIGINT);
                    break;
                case 's': // (aka. stat) dev util to show values of all application data (like habit_data, paint_info)
                    is_info_screen = !is_info_screen; 
                    if (is_info_screen) draw_info_screen(&data, &paint_info);
                    else {
                        draw_screen(&data, &paint_info);
                        draw_calendar(&data, &paint_info);
                    }
                    break;
            }
        }
    }

    save_data(&data);
    free_allocated_data(&data);
}

int main(int argc, char *argv[]) {
    run();
    return 0;
}
