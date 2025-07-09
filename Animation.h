#ifndef ANIMATION_H
#define ANIMATION_H

// Struktur für eine Animation
typedef struct {
    const char* const* const* frames; // Array von Frame-Strings
    int num_frames;            // Anzahl der Frames
    int rows;                   // Zeilen 
    int cols;                  // Spalen
} Pictures;

// Animationsfunktion 2 nebeneinander
void animate_dual(const Pictures* pictures1, const Pictures* pictures2, int Feuchte, int delay_ms);



#endif