#include "Game.h"

Game::Game() : grid(), incomingPiece() {

    /*Đọc highscore*/
    Reset();
}

void Game::Reset() {
    grid = Grid();
    incomingPiece = Piece();

    level = 1;
    score = 0;

    gravitySpeed = 20;
    activePiece.SetPosition({0, 0});

    pause = false;

    begin = true;
    pieceActivating = false;
    lineDeleting = false;
    gameover = false; // Rất quan trọng: đặt lại trạng thái gameover

    gravityMovementCounter = 0;
    lateralMovementCounter = 0;
    turnMovementCounter = 0;
    fastFallMovementCounter = 0;
    fadeLineCounter = 0; // Đừng quên reset biến đếm cho hiệu ứng fading
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

                for (int k = 1; k < HORIZONTAL_GRID_SIZE - 1; ++k) {
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

            for (int i = 1; i < HORIZONTAL_GRID_SIZE - 1; ++i) {
                grid.SetSquare(i, j, EMPTY);
            }
            
            for (int j2 = j - 1; j2 >= 0; --j2) {
                for (int i2 = 1; i2 < HORIZONTAL_GRID_SIZE - 1; ++i2) {
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

            ++deletedLineCount;
        }
    }
    return deletedLineCount;
}

void Game::UpdateFalling() {
    if (CheckCollision()) {
        s.PlaySoundN(PIECE_LANDED);
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

            activePiece.SetPosition({ activePiece.GetPosition().x - 1, activePiece.GetPosition().y });
        }
        
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

            activePiece.SetPosition({ activePiece.GetPosition().x + 1, activePiece.GetPosition().y });
        }
    }
        
    return collision;
}

bool Game::UpdateTurningMovement() {

    if (IsKeyDown(KEY_UP))
    {

        Square rotatedMatrix[4][4];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                rotatedMatrix[i][j] = activePiece.GetSquare(i, j);


        Square aux;
        aux = rotatedMatrix[0][0];
        rotatedMatrix[0][0] = rotatedMatrix[3][0];
        rotatedMatrix[3][0] = rotatedMatrix[3][3];
        rotatedMatrix[3][3] = rotatedMatrix[0][3];
        rotatedMatrix[0][3] = aux;

        aux = rotatedMatrix[1][0];
        rotatedMatrix[1][0] = rotatedMatrix[3][1];
        rotatedMatrix[3][1] = rotatedMatrix[2][3];
        rotatedMatrix[2][3] = rotatedMatrix[0][2];
        rotatedMatrix[0][2] = aux;

        aux = rotatedMatrix[2][0];
        rotatedMatrix[2][0] = rotatedMatrix[3][2];
        rotatedMatrix[3][2] = rotatedMatrix[1][3];
        rotatedMatrix[1][3] = rotatedMatrix[0][1];
        rotatedMatrix[0][1] = aux;

        aux = rotatedMatrix[1][1];
        rotatedMatrix[1][1] = rotatedMatrix[2][1];
        rotatedMatrix[2][1] = rotatedMatrix[2][2];
        rotatedMatrix[2][2] = rotatedMatrix[1][2];
        rotatedMatrix[1][2] = aux;

        Vector2 piecePos = activePiece.GetPosition();
        int piecePositionX = (int)piecePos.x;
        int piecePositionY = (int)piecePos.y;
        bool valid = true;
        for (int i = 0; i < 4 && valid; ++i) {
            for (int j = 0; j < 4 && valid; ++j) {
                if (rotatedMatrix[i][j] == FALLING) {
                    int gridX = piecePositionX + i;
                    int gridY = piecePositionY + j;
                    if (gridX < 1 || gridX >= HORIZONTAL_GRID_SIZE - 1 ||
                        gridY < 0 || gridY >= VERTICAL_GRID_SIZE - 1 ||
                        (grid.GetSquare(gridX, gridY) == FULL || grid.GetSquare(gridX, gridY) == BLOCK)) {
                        valid = false;
                    }
                }
            }
        }

        if (valid) {

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    activePiece.SetSquare(i, j, rotatedMatrix[i][j]);

            for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; j--)
            {
                for (int i = 1; i < HORIZONTAL_GRID_SIZE - 1; i++)
                {
                    if (grid.GetSquare(i, j) == FALLING)
                    {
                        grid.SetSquare(i, j, EMPTY);
                    }
                }
            }

            for (int i = piecePositionX; i < piecePositionX + 4; i++)
            {
                for (int j = piecePositionY; j < piecePositionY + 4; j++)
                {
                    if (activePiece.GetSquare(i - piecePositionX, j - piecePositionY) == FALLING)
                    {
                        if (i >= 0 && i < HORIZONTAL_GRID_SIZE && j >= 0 && j < VERTICAL_GRID_SIZE) {
                            grid.SetSquare(i, j, FALLING);
                        }
                    }
                }
            }
            return true;
        }

        return false;
    }

    return false;
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

                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                        s.PlaySoundN(MOVE);
                        lateralMovementCounter = LATERAL_SPEED;
                    }
                    if (IsKeyPressed(KEY_UP)) {
                        s.PlaySoundN(ROTATE);
                        turnMovementCounter = TURNING_SPEED;
                    }
                    if (IsKeyDown(KEY_DOWN) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER))
                    {
                        gravityMovementCounter += gravitySpeed /*Gravity Speed*/;
                    }

                    if (gravityMovementCounter >= gravitySpeed) {

                        UpdateFalling();
                        CheckCompletedLine();
                        gravityMovementCounter = 0;
                    }

                    if (lateralMovementCounter >= LATERAL_SPEED)
                    {
                        if (!UpdateSideMovement()) lateralMovementCounter = 0;
                    }

                    if (turnMovementCounter >= TURNING_SPEED)
                    {

                        if (UpdateTurningMovement()) turnMovementCounter = 0;
                    }
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

                if (fadeLineCounter % 8 < 4) fadingColor = BLACK;
                else fadingColor = LIGHTGRAY;

                if (fadeLineCounter >= FADING_TIME)
                {
                    int deletedLines = 0;
                    deletedLines = UpdateCompletedLine();
                    fadeLineCounter = 0;
                    lineDeleting = false;

                    float multiply = 0.8f + 0.2f * (float)level;

                    switch (deletedLines) {
                        case 1: { 
                            score += static_cast<int>(100.0f * multiply);
                            s.PlaySoundN(LINE_CLEAR);
                            break; 
                        }
                        case 2: { 
                            score += static_cast<int>(300.0f * multiply); 
                            s.PlaySoundN(LINE_CLEAR);
                            break; 
                        }
                        case 3: { 
                            score += static_cast<int>(500.0f * multiply); 
                            s.PlaySoundN(LINE_CLEAR);
                            break; 
                        }
                        case 4: { 
                            score += static_cast<int>(800.0f * multiply); 
                            s.PlaySoundN(FOUR_LINES_CLEAR);
                            break; 
                        }
                        default: break;
                    }

                    if (score > 200 && level == 1) level++;
                    else if (score > 400 && level == 2) level++;
                    else if (score > 800 && level == 3) level++;
                    else if (score > 1200 && level == 4) level++;
                    
                    gravitySpeed = 20 + 10 * level;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Reset();
        }
    }
}

void Game::DrawGame() {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameover) {

            Vector2 offset;
            offset.x = (screenWidth - (HORIZONTAL_GRID_SIZE * SQUARE_SIZE)) / 2;
            offset.y = (screenHeight - ((VERTICAL_GRID_SIZE - 5) * SQUARE_SIZE)) / 2;

            int controller = offset.x;
            for (int j = 0; j < VERTICAL_GRID_SIZE; j++)
            {
                for (int i = 0; i < HORIZONTAL_GRID_SIZE; i++)
                {

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

            offset.x = (screenWidth - (HORIZONTAL_GRID_SIZE * SQUARE_SIZE)) / 2;
            offset.y = 40;

            controller = offset.x;

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

                offset.x = controller;
                offset.y += SQUARE_SIZE;
            }

            DrawText("INCOMING:", controller, 20, 14, GRAY);
            DrawText(TextFormat("SCORE: %04i", score ), controller, offset.y + 10, 14, GRAY);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, screenHeight/2 - 50, 20, GRAY);
    EndDrawing();
}

void Game::UpdateDrawGame() {
    UpdateGame();
    DrawGame();
}