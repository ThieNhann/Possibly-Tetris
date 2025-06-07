#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include "Sounds.h"
#include "Config.h"
#include "memory"
#include "functional"

enum ButtonState {
    DEFAULT,
    HOVERED,
    CLICKED
};

class Button {
protected:
    Rectangle hitbox;
    Texture2D texture;
    ButtonState state;
    std::function<void()> onClickCallback;

    int holdCounter = 0;
    const int initialDelay = 15;
    const int repeatRate = 5;
public:

    Button(Vector2 pos, float width, float height, Texture2D defaultTexture, std::function<void()> callback = nullptr);

    virtual ~Button();

    Vector2 GetPosition() const;
    Rectangle GetHitbox() const;
    ButtonState GetState() const;


    virtual void Update(); 
    virtual void Draw();
    void OnClick();

    bool IsMouseOver() const;
};

enum VolumeState {
    HIGH,
    LOW,
    MUTED
};

class VolumeButton {
private:
    Rectangle hitbox;
    Texture2D texture;
    ButtonState state;
    Texture2D mutedTexture;
    Texture2D volumeLowTexture;
    Texture2D volumeHighTexture;
    VolumeState volumeState;
public:
    VolumeButton(Vector2 pos);
    ~VolumeButton();
    void Update();
    void OnClick();
    bool IsMouseOver();
    void Draw();
private:
    void UpdateTexture();
};

#endif