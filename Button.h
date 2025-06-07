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

/*enum VolumeState {
    HIGH,
    LOW,
    MUTED
};

class VolumeButton : public Button {
private:
    Texture2D mutedTexture;
    Texture2D volumeLowTexture;
    Texture2D volumeHighTexture;
    VolumeState volumeState;

public:
    VolumeButton(Vector2 pos, float size);
    ~VolumeButton() override;
    void Update() override;
    void OnClick() override;
private:
    void UpdateTexture();
};

class PauseButton : public Button {
public:
    PauseButton(Vector2 pos);
    ~PauseButton() override;
    void Update() override;
    void OnClick(bool& gamePause) override;
}; */

#endif