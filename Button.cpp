#include "Button.h"

Button::Button(Vector2 pos, float width, float height, Texture2D defaultTexture, std::function<void()> callback)
    : hitbox({pos.x, pos.y, width, height}),
      texture(defaultTexture),
      state(DEFAULT),
      onClickCallback(callback) {
}

Button::~Button() {}

Vector2 Button::GetPosition() const {
    return {hitbox.x, hitbox.y};
}

Rectangle Button::GetHitbox() const {
    return hitbox;
}

ButtonState Button::GetState() const {
    return state;
}

void Button::Update() {
    if (IsMouseOver()) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            state = CLICKED;
            holdCounter++;

            if (holdCounter == 1 || (holdCounter > initialDelay && (holdCounter - initialDelay) % repeatRate == 0)) {
                OnClick();
            }
        } else {
            state = HOVERED;
            holdCounter = 0;
        }
    } else {
        state = DEFAULT;
        holdCounter = 0; 
    }
}


void Button::Draw() {
    DrawTexture(texture, (int)hitbox.x, (int)hitbox.y, WHITE);

    if (state == HOVERED) {
        DrawRectangleLines((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, DARKGRAY);
    } else if (state == CLICKED) {
        DrawRectangleLines((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, BLACK);
    }
}

void Button::OnClick() {
    if (onClickCallback) {
        onClickCallback();
    }
}

bool Button::IsMouseOver() const {
    return CheckCollisionPointRec(GetMousePosition(), hitbox);
}

/*--------------------------Volume Button---------------------------------*/

VolumeButton::VolumeButton(Vector2 pos) {
    hitbox = Rectangle{pos.x, pos.y, BUTTON_SIZE, BUTTON_SIZE};
    texture = LoadTexture("assets/image/volume2.png");
    state = DEFAULT;
    mutedTexture = LoadTexture("assets/image/mute.png");
    volumeLowTexture = LoadTexture("assets/image/volume1.png");
    volumeHighTexture = LoadTexture("assets/image/volume2.png");
    SetMasterVolume(1.0f);
    texture = volumeHighTexture;

}
    

VolumeButton::~VolumeButton() {
    UnloadTexture(mutedTexture);
    UnloadTexture(volumeLowTexture);
    UnloadTexture(volumeHighTexture);
}

bool VolumeButton::IsMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), hitbox);
}

void VolumeButton::Update() {
    if (IsMouseOver()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = CLICKED;
            OnClick();
        } else {
            state = HOVERED;
        }
    } else {
        state = DEFAULT;
    }
}

void VolumeButton::OnClick() {
    if (volumeState == HIGH) {
        volumeState = MUTED;
        SetMasterVolume(0.0f);
        texture = mutedTexture;
    } else if (volumeState == MUTED) {
        volumeState = LOW;
        SetMasterVolume(0.5f);
        texture = volumeLowTexture;
    } else {
        volumeState = HIGH;
        SetMasterVolume(1.0f);
        texture = volumeHighTexture;
    }
}

void VolumeButton::Draw() {
    DrawTexture(texture, (int)hitbox.x, (int)hitbox.y, WHITE);

    if (state == HOVERED) {
        DrawRectangleLines((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, BLUE);
    } else if (state == CLICKED) {
        DrawRectangleLines((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, BLACK);
    }
}

/*

PauseButton::PauseButton(Vector2 pos) : Button(pos, BUTTON_SIZE, BUTTON_SIZE, LoadTexture("assets/image/pause.png")) {}

PauseButton::~PauseButton() {
    UnloadTexture(texture);
}

void PauseButton::Update() {
    Button::Update();
}


void PauseButton::OnClick() {
    
}

*/