#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Piece.h"
#include <raylib.h>


class Game {
private:
    static Grid grid;
    static Piece activePiece;
    static Piece incomingPiece;
    static bool beginFlag;
public:
    void CreatePiece();
};


#endif