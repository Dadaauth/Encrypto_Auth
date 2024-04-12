#include <stdio.h>

void drawProgressBar(int percentage) {
    const int barWidth = 50;
    int progress = barWidth * percentage / 100;

    printf("[");
    for (int i = 0; i < barWidth; i++) {
        if (i < progress) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\r", percentage);
    fflush(stdout);  // Flush the output to ensure the progress bar is displayed immediately
}

int main() {
    for (int i = 0; i <= 100; i++) {
        drawProgressBar(i);
        // Simulate some work being done
        for (int j = 0; j < 10000000; j++) {
            // Do some computation
        }
    }
    printf("\n");

    return 0;
}
