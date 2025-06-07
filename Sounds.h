#ifndef SOUNDS_H
#define SOUNDS_H

#include <raylib.h>
#include <vector>

enum SoundName { LINE_CLEAR, ROTATE, PIECE_LANDED, CLICK };

class Sounds {
private:
    Sound line_clear;
    Sound rotate;
    Sound piece_landed;
    Sound click;
public:
    Sounds();
    ~Sounds();
    void PlaySoundN(SoundName);
    void SetVolume(float);
};

#endif