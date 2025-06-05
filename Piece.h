#ifndef PIECE_H
#define PIECE_H

#include <raylib.h>
#include <iostream>

typedef enum Square { FULL, FALLING, EMPTY, BLOCK, FADING } Square;

class Piece {
protected:
    Square pieceMatrix[4][4];
    Vector2 position;
public:
    Piece();
    Piece(Vector2 pos);
    void print() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (pieceMatrix[i][j] == FALLING) std::cout << "+";
                if (pieceMatrix[i][j] == EMPTY) std::cout << "-";
            }
            std::cout << "\n";
        }
    }
    virtual ~Piece() {}
public:
    void SetSquare(int, int, Square);
    Square GetSquare(int, int);
    void SetPosition(Vector2);
    Vector2 GetPosition();
    static Piece GetRandomPiece();
};

class CubePiece : public Piece {
public:
    CubePiece();
    CubePiece(Vector2);
};

class LPiece : public Piece {
public:
    LPiece();
    LPiece(Vector2);
};

class LInversaPiece : public Piece {
public:
    LInversaPiece();
    LInversaPiece(Vector2);
};

class Recta : public Piece {
public:
    Recta();
    Recta(Vector2);
};

class TPiece : public Piece {
public:
    TPiece();
    TPiece(Vector2);
};

class SPiece : public Piece {
public:
    SPiece();
    SPiece(Vector2);
};


class SInversaPiece : public Piece {
public:
    SInversaPiece();
    SInversaPiece(Vector2);
};


#endif