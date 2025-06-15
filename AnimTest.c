#include <stdio.h>
#include <stdlib.h>
#include "Animation.h"
#include <windows.h>

void animate_dual(const Pictures* pictures1, const Pictures* pictures2, int Feuchte, int delay_ms) {
    int max_frames = pictures1->num_frames > pictures2->num_frames ? pictures1->num_frames : pictures2->num_frames;
    int offset = pictures1->cols + 5;

    // Bildschirm am Anfang leeren
    printf("\033[H\033[J");

    for (int frame = 0; frame < max_frames; ++frame) {
        // Animation 1
        int f1 = frame % pictures1->num_frames;
        for (int row = 0; row < pictures1->rows; ++row) {
            printf("\033[%d;1H%s", row + 1, pictures1->frames[f1][row]);
        }

        // Animation 2
        int f2 = frame % pictures2->num_frames;
        for (int row = 0; row < pictures2->rows; ++row) {
            printf("\033[%d;%dH%s", row + 1, offset + 1, pictures2->frames[f2][row]);
        }

        // Variable über Animation 2 anzeigen (in Zeile 1, Spalte offset+1)
        printf("\033[1;%dHWasserstand: %d", offset + 1, Feuchte);

        // Delay
        Sleep(delay_ms);
    }
}
