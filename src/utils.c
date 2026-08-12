#include "utils.h"


volatile bool running = true;

void interrupt_handler(int signum) {
    write(STDOUT_FILENO, "\e[?25h", 6);
    system("tput rmcup && stty icanon echo");
    running = false;
}

struct winsize get_screen_size() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws;
}

void setup_terminal() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = interrupt_handler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
   
    system("tput smcup && stty -icanon -echo");
    write(STDOUT_FILENO, "\e[?25l", 6);
}

void move_cursor(int row, int col) {
    printf("\e[%d;%dH", row, col);
    fflush(stdout);
}


int max_label_width(char **labels, int labels_count) {
    int res = MIN_LABEL_WIDTH;

    for (int label_idx = 0; label_idx < labels_count; label_idx++) {
        for (int char_idx = 0; char_idx <= MAX_LABEL_LENGTH; char_idx++) {
            if (char_idx == MAX_LABEL_LENGTH) {
                return MAX_LABEL_LENGTH;
            }
            if (labels[label_idx][char_idx] == '\0') {
                if (res < char_idx) res = char_idx;
                break;
            }
        }
    }

    return res;
}

void print_error(const char *err) {
    dprintf(STDOUT_FILENO, "\e[999;1H\e[2K\e[31mERROR: %s\e[39m", err);
    fflush(stdout);
}


time_t ceil_timestamp_day(time_t timestamp) {
    return timestamp / SECONDS_IN_DAY * SECONDS_IN_DAY;
}

char get_char_from_weekday(int weekday_idx) {
    const char weekdays[7] = {'s', 'm', 't', 'w', 't', 'f', 's'};

    return weekdays[weekday_idx];
}

int get_weekday_from_timestamp(time_t *timestamp) {
    struct tm *time_info;
    time_info = localtime(timestamp);
    return time_info->tm_wday;
}


void change_bit(uint8_t *byte, int bit_idx) {
    uint8_t mask = 0x1 << bit_idx;
    *byte ^= mask;
}

int is_bit_true(uint8_t *byte, int bit_idx) {
    uint8_t mask = 0x1 << bit_idx;
    return *byte & mask;
}
