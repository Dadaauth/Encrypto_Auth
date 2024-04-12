#include <stdio.h>

void drawProgressBar(double percentage) {
    const int barWidth = 50;
    int progress = barWidth * percentage / 100;

    printf("[");
    for (int i = 0; i < barWidth; i++)
    {
        if (i < progress)
            printf("=");
        else
            printf(" ");
    }
    printf("] %0.2f%%", percentage);
    /* Check if the percentage is complete. This is needed so other outputs doesn't overwite the progress bar */
    if (percentage == 100)
        printf("\n");
    else
        printf("\r");
    fflush(stdout);
}