#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <string.h>

volatile sig_atomic_t done = 0;

void interrupt_handler(int signum) {
    printf("done!\n");
    done = 1;
}

struct winsize get_screen_size() {
    struct winsize ws;
    int fd;

    fd = open("/dev/tty", O_RDWR);
    if (fd < 0 || ioctl(fd, TIOCGWINSZ, &ws) < 0) err(8, "/dev/tty");
    close(fd);
    return ws;
}

void run() {
    struct winsize ws = get_screen_size();

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = interrupt_handler;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    
    int pid = getpid();

    while (!done) {
        printf("PID: %d\n", pid);
    }

    printf("finished properly!\n");
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
