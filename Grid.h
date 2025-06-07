#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include "Config.h"
#include "Piece.h"

class Grid {
private:
    static std::array<std::array<Square, VERTICAL_GRID_SIZE>, HORIZONTAL_GRID_SIZE> grid;
public:
    Grid();
    void AssignPiece(Vector2, Piece);
    Square GetSquare(int, int);
    void SetSquare(int, int, Square);
};

#endif