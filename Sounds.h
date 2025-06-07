#ifndef SOUNDS_H
#define SOUNDS_H

#include <raylib.h>
#include <vector>

enum SoundName { LINE_CLEAR, ROTATE, PIECE_LANDED, CLICK, FALL };

class Sounds {
private:
    Sound line_clear;
    Sound rotate;
    Sound piece_landed;
    Sound click;
    Sound fall;
public:
    Sounds();
    ~Sounds();
    void PlaySoundN(SoundName);
    void SetVolume(float);
};

#endif