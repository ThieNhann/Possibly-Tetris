#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include "Grid.h"
#include "Piece.h"
#include "Sounds.h"
#include "Button.h"
#include <raylib.h>
#include <fstream>


class Game {
private:
    Grid grid;
    Piece activePiece;
    Piece incomingPiece;
public:
    std::unique_ptr<Button> pauseButton;
public:
    Sounds s;
    bool begin;
    bool gameover;
    bool pause;
    bool pieceActivating;
    bool lineDeleting;
    int fastFallMovementCounter;
    int gravityMovementCounter;
    int lateralMovementCounter;
    int turnMovementCounter;
    int fadeLineCounter;
    Color fadingColor;
    int gravitySpeed;
    int level;
    int score;
    int highScore;
    int totalLinesCleared;
public: 
    Game();
    void Reset();
    void CreatePiece();
    bool CheckCollision();
    void CheckCompletedLine();
    int UpdateCompletedLine();
    void UpdateFalling();
    bool UpdateSideMovement();
    void UpdateGame();
    void DrawGame();
    void UpdateDrawGame();
    bool UpdateTurningMovement();
    void Harddrop();
    void LoadHighscore();
    void SaveHighscore();
};


#endif