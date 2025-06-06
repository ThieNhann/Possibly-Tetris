#include "Game.h"

Game::Game() : grid(), incomingPiece() {
    level = 1;
    lines = 0;

    activePiece.SetPosition({0, 0});

    pause = false;

    begin = true;
    pieceActivating = false;
    lineDeleting = false;

    gravityMovementCounter = 0;
    lateralMovementCounter = 0;
    turnMovementCounter = 0;
    fastFallMovementCounter = 0;

}

void Game::CreatePiece() {
    Vector2 pieceSpawnPosition = {(int)((HORIZONTAL_GRID_SIZE - 4) / 2), 0};

    if (begin) {
        begin = false;
        incomingPiece = Piece::GetRandomPiece();
    }

    activePiece.CopyPieceMatrix(incomingPiece);
    activePiece.SetPosition(pieceSpawnPosition);

    incomingPiece = Piece::GetRandomPiece();

    grid.AssignPiece(pieceSpawnPosition, activePiece);
    
}

bool Game::CheckCollision() {
    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
        for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {
            if (grid.GetSquare(i, j) == FALLING && ((grid.GetSquare(i, j + 1) == FULL) || grid.GetSquare(i, j + 1) == BLOCK)) {
                return true;
            }
        }
    }
    return false;
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
                fullSquareCount = 0;
                lineDeleting = true;

                for (int k = 0; k < HORIZONTAL_GRID_SIZE - 1; ++k) {

                    grid.SetSquare(k, j, FADING);

                }
            }

        }

    }

}

int Game::UpdateCompletedLine() {
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
    return deletedLineCount;
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
        pieceActivating = false;
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

            for (int i = HORIZONTAL_GRID_SIZE - 2; i >= 0; --i) {

                if (grid.GetSquare(i, j) == FALLING) {

                    if (i + 1 == HORIZONTAL_GRID_SIZE - 1 || grid.GetSquare(i + 1, j) == FULL) collision = true;
                }
            }
        }

        if (!collision) {

            for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
                    
                for (int i = HORIZONTAL_GRID_SIZE - 2; i >= 0; --i) {

                    if (grid.GetSquare(i, j) == FALLING) {

                        grid.SetSquare(i + 1, j, FALLING);
                        grid.SetSquare(i, j, EMPTY);
                    }
                }
            }
        }
        activePiece.SetPosition({ activePiece.GetPosition().x + 1, activePiece.GetPosition().y });
    }
    return collision;
}

void Game::UpdateGame() {
    if (!gameover) {

        if (IsKeyPressed('P')) pause = !pause;

        if (!pause) {

            if (!lineDeleting) {

                if (!pieceActivating) {

                    CreatePiece();
                    pieceActivating = true;
                    fastFallMovementCounter = 0;
                }
                else {

                    fastFallMovementCounter++;
                    gravityMovementCounter++;
                    lateralMovementCounter++;
                    turnMovementCounter++;

                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) lateralMovementCounter = LATERAL_SPEED;
                    if (IsKeyPressed(KEY_UP)) turnMovementCounter = TURNING_SPEED;
                    if (IsKeyDown(KEY_DOWN) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER))
                    {
                        gravityMovementCounter += 30 /*Gravity Speed*/;
                    }

                    if (gravityMovementCounter >= 30) {

                        UpdateFalling();
                        CheckCompletedLine();
                        gravityMovementCounter = 0;
                    }

                    if (lateralMovementCounter >= LATERAL_SPEED)
                    {
                        if (!UpdateSideMovement()) lateralMovementCounter = 0;
                    }

                    /*Turning Handle*/
                }

                for (int j = 0; j < 2; j++)
                {
                    for (int i = 1; i < HORIZONTAL_GRID_SIZE - 1; i++)
                    {
                        if (grid.GetSquare(i, j) == FULL)
                        {
                            gameover = true;
                        }
                    }
                }
            }
            else
            {
                fadeLineCounter++;

                if (fadeLineCounter % 8 < 4) fadingColor = MAROON;
                else fadingColor = GRAY;

                if (fadeLineCounter >= FADING_TIME)
                {
                    int deletedLines = 0;
                    deletedLines = UpdateCompletedLine();
                    fadeLineCounter = 0;
                    lineDeleting = false;

                    lines += deletedLines;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gameover = false;
        }
    }
}

void Game::DrawGame() {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameover) {

            Vector2 offset;
            offset.x = screenWidth/2 - (HORIZONTAL_GRID_SIZE*SQUARE_SIZE/2) - 50;
            offset.y = screenHeight/2 - ((VERTICAL_GRID_SIZE - 1)*SQUARE_SIZE/2) + SQUARE_SIZE*2;
            offset.y -= 50;

            int controller = offset.x;
            for (int j = 0; j < VERTICAL_GRID_SIZE; j++)
            {
                for (int i = 0; i < HORIZONTAL_GRID_SIZE; i++)
                {
                    // Draw each square of the grid
                    if (grid.GetSquare(i, j) == EMPTY)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY );
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid.GetSquare(i, j) == FULL)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GRAY);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid.GetSquare(i, j) == FALLING)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, DARKGRAY);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid.GetSquare(i, j) == BLOCK)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid.GetSquare(i, j) == FADING)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, fadingColor);
                        offset.x += SQUARE_SIZE;
                    }
                }
                offset.x = controller;
                offset.y += SQUARE_SIZE;
            }

            offset.x = 500;
            offset.y = 45;

            int controler = offset.x;

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (incomingPiece.GetSquare(i, j) == EMPTY)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY );
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY );
                        offset.x += SQUARE_SIZE;
                    }
                    else if (incomingPiece.GetSquare(i, j) == FALLING)
                    {
                        DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GRAY);
                        offset.x += SQUARE_SIZE;
                    }
                }

                offset.x = controler;
                offset.y += SQUARE_SIZE;
            }

            DrawText("INCOMING:", offset.x, offset.y - 100, 10, GRAY);
            DrawText(TextFormat("LINES:      %04i", lines), offset.x, offset.y + 20, 10, GRAY);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
    EndDrawing();
}

void Game::UpdateDrawGame() {
    UpdateGame();
    DrawGame();
}