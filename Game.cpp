#include "Game.h"

void Game::CreatePiece() {
    Vector2 pieceSpawnPosition = {(int)((HORIZONTAL_GRID_SIZE - 4) / 2), 0};

    if (beginFlag) {
        beginFlag = false;
        incomingPiece = Piece::GetRandomPiece();
    }

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            activePiece.CopyPieceMatrix(incomingPiece);
        }
    }

    incomingPiece = Piece::GetRandomPiece();

    grid.AssignPiece(pieceSpawnPosition, activePiece);
    
}

bool Game::CheckCollision() {
    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
        for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {
            if (grid.GetSquare(i, j) == FALLING && ((grid.GetSquare(i + 1, j) == FULL) || grid.GetSquare(i + 1, j) == BLOCK)) {
                return true;
            }
        }
    }
}

void Game::CheckCompletedLine() {
    int completedLineCount = 0;

    int fullSquareCount = 0;

    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

        fullSquareCount = 0;

        for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

            if (grid.GetSquare(i, j) == FULL) ++fullSquareCount;

            if (fullSquareCount == HORIZONTAL_GRID_SIZE - 2) {

                ++completedLineCount;

                for (int k = 0; k < HORIZONTAL_GRID_SIZE - 1; ++k) {

                    grid.SetSquare(k, j, FADING);

                }
            }

        }

    }

}

void Game::UpdateCompletedLine() {
    int deletedLineCount = 0;

    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
        
        while (grid.GetSquare(1, j) == FADING) {

            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                grid.SetSquare(i, j, EMPTY);
                
                for (int j2 = j - 1; j2 >= 0; --j2) {

                    for (int i2 = 0; i2 < HORIZONTAL_GRID_SIZE - 1; ++i2) {

                        if (grid.GetSquare(i2, j2) == FULL) {

                            grid.SetSquare(i2, j2 + 1, FULL);
                            grid.SetSquare(i2, j2, EMPTY); 
                        }

                        else if (grid.GetSquare(i2, j2) == FADING) {

                            grid.SetSquare(i2, j2 + 1, FADING);
                            grid.SetSquare(i2, j2, EMPTY);
                        }
                    }
                }
            }

            ++deletedLineCount;
        }
    }
}

void Game::UpdateFalling() {
    if (CheckCollision()) {
        for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                if (grid.GetSquare(i, j) == FALLING) {
                    grid.SetSquare(i, j, FULL);
                }
            }
        }
    }
    else {

        for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                if (grid.GetSquare(i, j) == FALLING) {
                    grid.SetSquare(i, j + 1, FALLING);
                    grid.SetSquare(i, j, EMPTY);
                }
            }
        }

        activePiece.SetPosition({ activePiece.GetPosition().x, activePiece.GetPosition().y + 1 });
    }
}

bool Game::UpdateSideMovement() {
    bool collision = false;

    if (IsKeyDown(KEY_LEFT)) {

        for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                if (grid.GetSquare(i, j) == FALLING) {
                    
                    if (i - 1 == 0 || grid.GetSquare(i - 1, j) == FULL) collision = true;

                }
            }
        }

        if (!collision) {

            for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

                for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                    if (grid.GetSquare(i, j) == FALLING) {
                        grid.SetSquare(i - 1, j, FALLING);
                        grid.SetSquare(i, j, EMPTY);
                    }
                }
            }
        }

        activePiece.SetPosition({ activePiece.GetPosition().x - 1, activePiece.GetPosition().y });
    }

    else if (IsKeyDown(KEY_RIGHT)) {

        for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                if (grid.GetSquare(i, j) == FALLING) {

                    if (i + 1 == HORIZONTAL_GRID_SIZE - 1 || grid.GetSquare(i + 1, j) == FULL) collision = true;
                }
            }
        }

        if (!collision) {

            for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {

                for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

                    if (grid.GetSquare(i, j) == FALLING) {

                        grid.SetSquare(i + 1, j, FALLING);
                        grid.SetSquare(i, j, FULL);
                    }
                }
            }
        }
        activePiece.SetPosition({ activePiece.GetPosition().x + 1, activePiece.GetPosition().y });
    }
}