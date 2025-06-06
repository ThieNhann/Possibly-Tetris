#include "Sounds.h"

Sounds::Sounds() {
    four_lines_clear = LoadSound("resources/audio/4_lines.wav");
    line_clear = LoadSound("resources/audio/line_clear.wav");
    move = LoadSound("resources/audio/move.wav");
    piece_landed = LoadSound("resources/audio/piece_landed.wav");
    rotate = LoadSound("resources/audio/rotate.wav");
    click = LoadSound("resources/audio/click.wav");
}

Sounds::~Sounds() {
    UnloadSound(four_lines_clear);
    UnloadSound(line_clear);
    UnloadSound(move);
    UnloadSound(piece_landed);
    UnloadSound(rotate);
}

void Sounds::PlaySoundN(SoundName name) {
    switch (name) {
        case (FOUR_LINES_CLEAR) : {
            PlaySound(four_lines_clear);
            break;
        }
        case (LINE_CLEAR) : {
            PlaySound(line_clear);
            break;
        }
        case (MOVE) : {
            PlaySound(move);
            break;
        }
        case (PIECE_LANDED) : {
            PlaySound(piece_landed);
            break;
        }
        case (ROTATE) : {
            PlaySound(rotate);
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