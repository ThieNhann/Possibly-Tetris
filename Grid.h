#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include "Config.h"
#include "Piece.h"

class Grid {
private:
    static Square grid[HORIZONTAL_GRID_SIZE][VERTICAL_GRID_SIZE];
public:
    Grid();
    void AssignPiece(Vector2, Piece);
    Square GetSquare(int, int);
    void SetSquare(int, int, Square);
};

#endif