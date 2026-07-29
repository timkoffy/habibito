void draw_screen(unsigned short cols, unsigned short rows) {
    rows /= 2;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(j == 0 || j == cols - 1) { 
                write(STDOUT_FILENO, "|", 1);
                continue;
            }
            if(i == 0 || i == rows - 1) {
                write(STDOUT_FILENO, "-", 1);
                continue;
            }
            write(STDOUT_FILENO, " ", 1);
        }
    }
}

void update_screen(char *buf) {
    // write(STDOUT_FILENO, "\e[3;4H", 6);
    char ch = 'h';
    if (buf[0] == 'g') write(STDOUT_FILENO, &ch, 1);
    write(STDOUT_FILENO, buf, 10);
    fflush(stdout);
}
