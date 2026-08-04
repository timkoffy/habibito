#include "utils.h"

/* terminal utils */

volatile short running = 1;

void interrupt_handler(int signum) {
    write(STDOUT_FILENO, "\e[?25h", 6);
    system("tput rmcup && stty icanon echo");
    running = 0;
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

void move_cursor(int col, int row) {
    printf("\e[%d;%dH", row, col);
    fflush(stdout);
}

/* visual utils */

unsigned short max_label_width(char **labels, unsigned short labels_count) {
    unsigned short res = MIN_LABEL_WIDTH;

    for (unsigned short i = 0; i < labels_count; i++) {
        for (unsigned short j = 0; j <= MAX_LABEL_LENGTH; j++) {
            if (j == MAX_LABEL_LENGTH) {
                return MAX_LABEL_LENGTH;
            }
            if (labels[i][j] == '\0') {
                if (res < j) res = j;
                break;
            }
        }
    }

    return res;
}

/* time visual utils */

time_t ceil_timestamp_day(time_t timestamp) {
    return timestamp / SECONDS_IN_DAY * SECONDS_IN_DAY;
}

char get_char_from_weekday(unsigned short wd_idx) {
    const char weekdays[7] = {'s', 'm', 't', 'w', 't', 'f', 's'};

    return weekdays[wd_idx];
}

int get_weekday_from_timestamp(time_t *timestamp) {
    struct tm *time_info;
    time_info = localtime(timestamp);
    return time_info->tm_wday;
}

/* statistics utils */

void print_error(const char *err) {
    dprintf(STDOUT_FILENO, "\e[999;1H\e[2K\e[31mERROR: %s\e[39m", err);
    fflush(stdout);
}

/* data utils */

void change_bit(unsigned char *byte, int bit_idx) {
    unsigned char mask = 0x1 << bit_idx;
    *byte ^= mask;
}

int is_bit_true(unsigned char *byte, int bit_idx) {
    unsigned char mask = 0x1 << bit_idx;
    return *byte & mask;
}
