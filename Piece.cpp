#include "Piece.h"

Piece::Piece() {}

Piece::Piece(Vector2 pos) : position(pos) {}

void Piece::SetSquare(int i, int j, Square state) {
    if (i <= 3 && i >= 0 && j <= 3 &&  j >= 0) {
        pieceMatrix[i][j] = state;
    }
}

Piece Piece::GetRandomPiece()  {

    int random = GetRandomValue(0, 6);

    Piece piece;

    for (int i = 0; i < 4; ++i) {

        for (int j = 0; j < 4; ++j) {

            piece.SetSquare(i, j, EMPTY);

        }

    }

    switch (random) {

        case 0: {
            piece.SetSquare(1, 1, FALLING);
            piece.SetSquare(1, 2, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(2, 2, FALLING);
        } break;

        case 1: {
            piece.SetSquare(1, 0, FALLING);
            piece.SetSquare(1, 1, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(2, 2, FALLING);
        } break;

        case 2: {
            piece.SetSquare(2, 0, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(2, 2, FALLING);
            piece.SetSquare(1, 2, FALLING);
        } break;

        case 3: {
            piece.SetSquare(0, 1, FALLING);
            piece.SetSquare(1, 1, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(3, 1, FALLING);
        } break;

        case 4: {
            piece.SetSquare(1, 0, FALLING);
            piece.SetSquare(1, 1, FALLING);
            piece.SetSquare(1, 2, FALLING);
            piece.SetSquare(2, 1, FALLING);
        } break;

        case 5: {
            piece.SetSquare(1, 1, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(2, 2, FALLING);
            piece.SetSquare(3, 2, FALLING);
        } break;

        case 6: {
            piece.SetSquare(1, 2, FALLING);
            piece.SetSquare(2, 2, FALLING);
            piece.SetSquare(2, 1, FALLING);
            piece.SetSquare(3, 1, FALLING);
        } break;
        
    }
}