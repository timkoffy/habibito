#include <stdio.h>

#include "painter.h"
#include "utils.h"
#include "data_manager.h"

void run() {
    setup_terminal();
    struct winsize ws = get_screen_size();

    struct habit_data data = load_mock_data();

    draw_screen(ws.ws_col, ws.ws_row, &data);
    fflush(stdout);
 
    while (running) {
    }
}

int main(int argc, char *argv[]) {
    int labels_count = 3;
    int max_label_length = 20;
    char **labels = malloc(sizeof(char*) * labels_count);
    for (int i = 0; i < labels_count; i++) {
        labels[i] = malloc(sizeof(char) * max_label_length);
    }

    char str1[] = "linux";
    char str2[] = "mus";
    char str3[] = "stretching";
    memcpy(labels[0], str1, sizeof(str1));
    memcpy(labels[1], str2, sizeof(str2));
    memcpy(labels[2], str3, sizeof(str3));

    printf("%s\n", labels[0]);
    printf("%d\n", labels[0][5]);

    printf("%d", max_label_width(labels, labels_count, max_label_length));

    // run();

    return 0;
}
