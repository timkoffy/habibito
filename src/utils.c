volatile short running = 1;

void interrupt_handler(int signum) {
    write (STDOUT_FILENO, "\e[?25h", 6);
    system("tput rmcup && stty echo");
    running = 0;
}

struct winsize get_screen_size() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) err(8, "/dev/tty");
    return ws;
}

void setup_terminal() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = interrupt_handler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
   
    system("tput smcup && stty -echo");
    write (STDOUT_FILENO, "\e[?25l", 6);
}
