#include "painter.h"

void draw_screen(unsigned short cols, unsigned short rows, struct habit_data *data) {
    const unsigned short TOTAL_HEIGHT = HEADER_HEIGHT + data->labels_count;
    for (int i = 0; i < rows; i++) {
        if (i >= TOTAL_HEIGHT) {
            printf("\n");
            continue;
        }

        for (int j = 0; j < cols; j++) {
            if (i == HEADER_HEIGHT - 1) { 
                printf("-");
                continue;
            }
            if (j == 0 && i >= HEADER_HEIGHT) {
                printf("%d ", i - HEADER_HEIGHT);
                j++;
                continue;
            }
            printf(" ");
        }
    }
}

