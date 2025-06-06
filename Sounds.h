#ifndef SOUNDS_H
#define SOUNDS_H

#include <raylib.h>
#include <vector>

enum SoundName { FOUR_LINES_CLEAR, LINE_CLEAR, MOVE, PIECE_LANDED, ROTATE, CLICK };

class Sounds {
private:
    Sound four_lines_clear;
    Sound line_clear;
    Sound move;
    Sound piece_landed;
    Sound rotate;
    Sound click;
public:
    Sounds();
    ~Sounds();
    void PlaySoundN(SoundName);
    void SetVolume(float);
};

#endif