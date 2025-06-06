#include "Grid.h"

void Grid::AssignPiece(Vector2 pos, Piece piece) {
    for (int i = pos.x; i < pos.x + 4; ++i) {
        for (int j = pos.y; j < pos.y + 4; ++j) {
            grid[i][j] = piece.GetSquare(i - pos.x, j - pos.y);
        }
    }
}

Square Grid::GetSquare(int i, int j) {
    return grid[i][j];
}

void Grid::SetSquare(int i, int j, Square state) {
    grid[i][j] = state;
}

