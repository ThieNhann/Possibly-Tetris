#include "Sounds.h"

Sounds::Sounds() {
    line_clear = LoadSound("resources/audio/line_clear.wav");
    rotate = LoadSound("resources/audio/rotate.wav");
    piece_landed = LoadSound("resources/audio/piece_landed.wav");
    click = LoadSound("resources/audio/click.wav");
}

Sounds::~Sounds() {
    UnloadSound(line_clear);
    UnloadSound(rotate);
    UnloadSound(piece_landed);
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
        default: break;
    }
}

void Sounds::SetVolume(float v) {
    SetMasterVolume(v);
} 