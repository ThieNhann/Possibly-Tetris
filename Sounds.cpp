#include "Sounds.h"

Sounds::Sounds() {
    line_clear = LoadSound("assets/audio/line_clear.wav");
    rotate = LoadSound("assets/audio/rotate.wav");
    piece_landed = LoadSound("assets/audio/piece_landed.wav");
    click = LoadSound("assets/audio/click.wav");
    fall = LoadSound("assets/audio/fall.wav");
}

Sounds::~Sounds() {
    UnloadSound(line_clear);
    UnloadSound(rotate);
    UnloadSound(piece_landed);
    UnloadSound(fall);
}

void Sounds::PlaySoundN(SoundName name) {
    switch (name) {
        case (LINE_CLEAR) : {
            PlaySound(line_clear);
            break;
        }
        case (ROTATE) : {
            PlaySound(rotate);
            break;
        }
        case (PIECE_LANDED) : {
            PlaySound(piece_landed);
            break;
        }
        case (CLICK) :   {
            PlaySound(click);
            break;
        }
        case (FALL) : {
            PlaySound(fall);
            break;
        }
        default: break;
    }
}

void Sounds::SetVolume(float v) {
    SetMasterVolume(v);
} 