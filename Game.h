#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include "Grid.h"
#include "Piece.h"
#include <raylib.h>


class Game {
private:
    Grid grid;
    Piece activePiece;
    Piece incomingPiece;
public:
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
    int lines;
    int level;
public: 
    Game();
    void CreatePiece();
    bool CheckCollision();
    void CheckCompletedLine();
    int UpdateCompletedLine();
    void UpdateFalling();
    bool UpdateSideMovement();
    void UpdateGame();
    void DrawGame();
    void UpdateDrawGame();
};


#endif