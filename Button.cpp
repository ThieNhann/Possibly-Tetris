#include "Button.h"

VolumeButton::VolumeButton(Vector2 pos) {
    muted = LoadTexture("resources/image/mute.png");
    volumeLow = LoadTexture("resources/image/volume1.png");
    volumeHigh = LoadTexture("resources/image/volume2.png");
    hitbox = {pos.x, pos.y, SQUARE_SIZE, SQUARE_SIZE};
    state = HIGH;
    texture = volumeHigh;
}

VolumeButton::~VolumeButton() {
    UnloadTexture(muted);
    UnloadTexture(volumeLow);
    UnloadTexture(volumeHigh);
}

Vector2 VolumeButton::GetPosition() {
    return {hitbox.x, hitbox.y};
}

void VolumeButton::Update() {     
    if (state == HIGH) {
        state = MUTED;
        texture = muted;
        SetMasterVolume(0.0f);
    }
    else if (state == MUTED) {
        state = LOW;
        texture = volumeLow;
        SetMasterVolume(0.5f);
    }
    else {
        state = HIGH;
        texture = volumeHigh;
        SetMasterVolume(1.0f);
    }
}

void VolumeButton::Draw() {
    DrawTexture(texture, hitbox.x, hitbox.y, WHITE);
}