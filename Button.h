#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include "Sounds.h"
#include "Config.h"

enum VolumeButtonState {MUTED, LOW, HIGH};
class VolumeButton {
private:
    Texture2D texture;
    Rectangle hitbox;
    VolumeButtonState state;
public:
    Texture2D muted;
    Texture2D volumeLow;
    Texture2D volumeHigh;
public:
    VolumeButton(Vector2);
    ~VolumeButton();
    Vector2 GetPosition();
    void Update();
    void Draw();
};

#endif