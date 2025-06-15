#include <stdio.h>
#include <unistd.h>
#include "Animation.h"
#include <ncurses.h>

void animate(const Pictures* pictures, int delay_ms) {
    initscr();           // ncurses-Modus starten
    noecho();            // Eingaben nicht anzeigen
    curs_set(0);         // Cursor ausblenden

    for (int frame = 0; frame < pictures->num_frames; ++frame) {
        clear();         // Bildschirm löschen
        for (int row = 0; row < pictures->rows; ++row) {
            mvprintw(row, 0, "%s", pictures->frames[frame][row]);
        }
        refresh();       // Änderungen anzeigen
        napms(delay_ms); // Pause in Millisekunden
    }

    curs_set(1);         // Cursor wieder einblenden
    endwin();            // ncurses-Modus beenden
}

void animate_dual(const Pictures* pictures1, const Pictures* pictures2, int Feuchte, int delay_ms) {
    initscr();
    noecho();
    curs_set(0);

    int max_frames = pictures1->num_frames > pictures2->num_frames ? pictures1->num_frames : pictures2->num_frames;
    int offset = pictures1->cols + 5; 

    for (int frame = 0; frame < max_frames; ++frame) {
        clear();

        // Animation 1
        int f1 = frame % pictures1->num_frames;
        for (int row = 0; row < pictures1->rows; ++row) {
            mvprintw(row, 0, "%s", pictures1->frames[f1][row]);
        }

        // Animation 2
        int f2 = frame % pictures2->num_frames;
        for (int row = 0; row < pictures2->rows; ++row) {
            mvprintw(row, offset, "%s", pictures2->frames[f2][row]);
        }

         // Variable über Animation 2 anzeigen
        mvprintw(0, offset, "Wasserstand: %d", Feuchte);

        refresh();
        napms(delay_ms);
    }

    curs_set(1);
    endwin();
}