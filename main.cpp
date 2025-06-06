#include <iostream>
#include <raylib.h> 
#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "Config.h"

int main () {
    InitWindow(screenWidth, screenHeight, "Possibly Tetris");

    Game game;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.UpdateDrawGame();
    }

    CloseWindow();

    return 0;
}