#include "utils.h"

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
