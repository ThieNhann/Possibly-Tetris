#include "Piece.h"

Piece::Piece() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            pieceMatrix[i][j] = EMPTY;
        }
    }
}

Piece::Piece(Vector2 pos) : Piece() {
    position = pos;
}

void Piece::SetSquare(int i, int j, Square state) {
    if (i >= 0 && i <= 3 && j >= 0 && j <= 4) {
        pieceMatrix[i][j] = state;
    }
}

Square Piece::GetSquare(int i, int j) {
    return pieceMatrix[i][j];
}

void Piece::SetPosition(Vector2 pos) {
    position = pos;
}

Vector2 Piece::GetPosition() {
    return position;
}


Piece Piece::GetRandomPiece() {
    int random = GetRandomValue(0, 6);

    switch (random) {
        case 0: return CubePiece();
        case 1: return LPiece();
        case 2: return LInversaPiece();
        case 3: return Recta();
        case 4: return TPiece();
        case 5: return SPiece();
        case 6: return SInversaPiece();
        default: return Piece();
    }
}

void Piece::CopyPieceMatrix(Piece other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            pieceMatrix[i][j] = other.pieceMatrix[i][j];
        }
    }
}


/*------------------------Derived Pieces------------------------*/

CubePiece::CubePiece() {
    SetSquare(1, 1, FALLING);
    SetSquare(1, 2, FALLING);
    SetSquare(2, 1, FALLING);
    SetSquare(2, 2, FALLING);
}

CubePiece::CubePiece(Vector2 pos) : CubePiece() {
    position = pos;
}

LPiece::LPiece() {
    SetSquare(1, 0, FALLING);
    SetSquare(1, 1, FALLING);
    SetSquare(1, 2, FALLING);
    SetSquare(2, 2, FALLING);
}

LPiece::LPiece(Vector2 pos) : LPiece() {
    position = pos;
}

LInversaPiece::LInversaPiece() {
    SetSquare(2, 0, FALLING);
    SetSquare(2, 1, FALLING);
    SetSquare(2, 2, FALLING);
    SetSquare(1, 2, FALLING);
}

LInversaPiece::LInversaPiece(Vector2 pos) : LInversaPiece() {
    position = pos;
}

Recta::Recta() {
    SetSquare(0, 1, FALLING);
    SetSquare(1, 1, FALLING);
    SetSquare(2, 1, FALLING);
    SetSquare(3, 1, FALLING);
}

Recta::Recta(Vector2 pos) : Recta() {
    position = pos;
}   

TPiece::TPiece() {
    SetSquare(1, 0, FALLING);
    SetSquare(1, 1, FALLING);
    SetSquare(1, 2, FALLING);
    SetSquare(2, 1, FALLING);
}

TPiece::TPiece(Vector2 pos) : TPiece() {
    position = pos;
}

SPiece::SPiece() {
    SetSquare(1, 1, FALLING);
    SetSquare(2, 1, FALLING);
    SetSquare(2, 2, FALLING);
    SetSquare(3, 2, FALLING);
}

SPiece::SPiece(Vector2 pos) : SPiece() {
    position = pos;
}

SInversaPiece::SInversaPiece() {
    SetSquare(1, 2, FALLING);
    SetSquare(2, 2, FALLING);
    SetSquare(2, 1, FALLING);
    SetSquare(3, 1, FALLING);
}

SInversaPiece::SInversaPiece(Vector2 pos) : SInversaPiece() {
    position = pos;
}
