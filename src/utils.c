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
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = interrupt_handler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
   
    system("tput smcup && stty -icanon -echo");
    write(STDOUT_FILENO, "\e[?25l", 6);
}
