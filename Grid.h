#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include "Config.h"
#include "Piece.h"

class Grid {
private:
    static Square grid[HORIZONTAL_GRID_SIZE][VERTICAL_GRID_SIZE];
public:
    void AssignPiece(Vector2, Piece);
};

#endif