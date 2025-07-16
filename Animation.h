#ifndef ANIMATION_H
#define ANIMATION_H

// Struktur für eine Animation
typedef struct {
    const char* const* const* frames;   // Array von Frame-Strings
    int num_frames;                     // Anzahl der Frames
    int rows;                           // Zeilen 
    int cols;                           // Spalen
} Pictures;

// Animationsfunktion 2 nebeneinander
void AnimateDual(const Pictures* Pictures1, const Pictures* Pictures2, int Feuchte, int DelayMs);

#endif