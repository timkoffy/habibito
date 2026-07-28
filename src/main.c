#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

struct winsize get_screen_size() {
    struct winsize ws;
    int fd;

    fd = open("/dev/tty", O_RDWR);
    if (fd < 0 || ioctl(fd, TIOCGWINSZ, &ws) < 0) err(8, "/dev/tty");
    close(fd);
    return ws;
}

void run() {
    struct winsize ws;

    for(;;) { 
        ws = get_screen_size();

        printf("%d, %d\n", ws.ws_row, ws.ws_col);
    }
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
