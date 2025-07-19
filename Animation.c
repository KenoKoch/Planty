#include <stdio.h>
#include <unistd.h>
#include "Animation.h"
#include <ncurses.h>


void AnimateDual(const Pictures* Pictures1, const Pictures* Pictures2, int Feuchte, int DelayMs) {
    initscr();
    noecho();
    curs_set(0);

    int MaxFrames = Pictures1->num_frames > Pictures2->num_frames ? Pictures1->num_frames : Pictures2->num_frames;
    int OffsetAnim1 = 6; 
    int OffsetAnim2 = Pictures1->cols + OffsetAnim1 + 5; 
    int OffsetAnim3 = OffsetAnim2 + 5;

    for (int Frame = 0; Frame < MaxFrames; ++Frame) {
        clear();

        // Animation 1
        int FrameNumber1 = Frame % Pictures1->num_frames;
        for (int Row = 0; Row < Pictures1->rows; ++Row) {
            mvprintw(Row, OffsetAnim1, "%s", Pictures1->frames[FrameNumber1][Row]);
        }

        // Animation 2
        int FrameNumber2 = Frame % Pictures2->num_frames;
        for (int Row = 0; Row < Pictures2->rows; ++Row) {
            mvprintw(Row, OffsetAnim2, "%s", Pictures2->frames[FrameNumber2][Row]);
        }

         // Variable über Animation 2 
        mvprintw(0, OffsetAnim3, "%d", Feuchte);

        refresh();
        napms(DelayMs);
    }

    curs_set(1);
    endwin();
}
