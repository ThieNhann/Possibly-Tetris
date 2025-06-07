#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include "Grid.h"
#include "Piece.h"
#include "Sounds.h"
#include "Button.h"
#include <raylib.h>
#include <fstream>

enum RotateDirection { NONE, CLOCKWISE, COUNTERCLOCKWISE, R180 };
enum MoveDirection {NONEm,  LEFT, RIGHT};
class Game {
private:
    Grid grid;
    Piece activePiece;
    Piece incomingPiece;
public:
    std::unique_ptr<Button> pauseButton;
    VolumeButton volumeButton;
    std::unique_ptr<Button> rotateLeftButton;
    std::unique_ptr<Button> rotate180Button;
    std::unique_ptr<Button> rotateRightButton;
    std::unique_ptr<Button> moveLeftButton;
    std::unique_ptr<Button> moveRightButton;
    std::unique_ptr<Button> fastDropButton;
    std::unique_ptr<Button> hardDropButton;
public:
    RotateDirection requestedRotation;
    MoveDirection requestedMoveDirection;
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
    int fastFallSpeed;
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