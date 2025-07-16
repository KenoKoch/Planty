#include <stdio.h>
#include <unistd.h>
#include "Animation.h"
#include <ncurses.h>


void animate_dual(const Pictures* pictures1, const Pictures* pictures2, int Feuchte, int delay_ms) {
    initscr();
    noecho();
    curs_set(0);

    int max_frames = pictures1->num_frames > pictures2->num_frames ? pictures1->num_frames : pictures2->num_frames;
    int OffsetAnim1 = 5; 
    int OffsetAnim2 = pictures1->cols + OffsetAnim1 + 5; 

    for (int frame = 0; frame < max_frames; ++frame) {
        clear();

        // Animation 1
        int f1 = frame % pictures1->num_frames;
        for (int row = 0; row < pictures1->rows; ++row) {
            mvprintw(row, OffsetAnim1, "%s", pictures1->frames[f1][row]);
        }

        // Animation 2
        int f2 = frame % pictures2->num_frames;
        for (int row = 0; row < pictures2->rows; ++row) {
            mvprintw(row, OffsetAnim2, "%s", pictures2->frames[f2][row]);
        }

         // Variable über Animation 2 
        mvprintw(0, OffsetAnim2, "Wasserstand: %d", Feuchte);

        refresh();
        napms(delay_ms);
    }

    curs_set(1);
    endwin();
}
