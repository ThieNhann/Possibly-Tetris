#ifndef PIECE_H
#define PIECE_H

#include <raylib.h>

typedef enum Square { FULL, FALLING, EMPTY, BLOCK, FADING } Square;

class Piece {
private:
    Square pieceMatrix[4][4];
    Vector2 position;
public:
    Piece();
    Piece(Vector2 pos);
    void SetSquare(int, int, Square);
    static Piece GetRandomPiece();
};

#endif