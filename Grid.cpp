#include "Grid.h"

Square Grid::grid[HORIZONTAL_GRID_SIZE][VERTICAL_GRID_SIZE];

Grid::Grid() {
    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
        for (int i = 0; i <= HORIZONTAL_GRID_SIZE - 1; ++i) {
            if (i == 0 || i == HORIZONTAL_GRID_SIZE - 1 || j == VERTICAL_GRID_SIZE - 1) {
                grid[i][j] = BLOCK;
            }
            else grid[i][j] = EMPTY;
        }
    }
}
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

