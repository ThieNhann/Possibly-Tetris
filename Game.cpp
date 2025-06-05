#include "Game.h"

void Game::CreatePiece() {
    Vector2 pieceSpawnPosition = {(int)((HORIZONTAL_GRID_SIZE - 4) / 2), 0};

    if (beginFlag) {
        beginFlag = false;
        incomingPiece = Piece::GetRandomPiece();
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            activePiece.CopyPieceMatrix(incomingPiece);
        }
    }

    incomingPiece = Piece::GetRandomPiece();

    grid.AssignPiece(pieceSpawnPosition, activePiece);
    
}