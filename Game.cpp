#include "Game.h"

Game::Game() : vol({ screenWidth - 2 * SQUARE_SIZE, 2 * SQUARE_SIZE }) {

    LoadHighscore();
    Reset();
}

void Game::Reset() {
    grid = Grid();
    incomingPiece = Piece();
    level = 1;
    score = 0;
    totalLinesCleared = 0;

    gravitySpeed = 48;
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

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            if (activePiece.GetSquare(i, j) == FALLING) {
                grid.SetSquare(pieceSpawnPosition.x + i, pieceSpawnPosition.y + j, FALLING);
            }
        }
    }

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

        for (int i = 1; i < HORIZONTAL_GRID_SIZE - 1; ++i) {

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

    if (IsKeyDown(KEY_A)) {

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

    else if (IsKeyDown(KEY_D)) {

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

    Piece rotatedMatrix = activePiece;

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Q) || IsKeyDown(KEY_E)) {
        if (IsKeyDown(KEY_W))
        {
            rotatedMatrix.Rotate180();
        } else if (IsKeyDown(KEY_Q)) {
            rotatedMatrix.RotateCounterclockwise();
        }
        else if (IsKeyDown(KEY_E)) {
            rotatedMatrix.RotateClockwise();
        }

        Vector2 piecePos = activePiece.GetPosition();
        int piecePositionX = (int)piecePos.x;
        int piecePositionY = (int)piecePos.y;
        bool valid = true;
        for (int i = 0; i < 4 && valid; ++i) {
            for (int j = 0; j < 4 && valid; ++j) {
                if (rotatedMatrix.GetSquare(i, j) == FALLING) {
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
                activePiece = rotatedMatrix;

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
                    if (i >= 0 && i < HORIZONTAL_GRID_SIZE - 1 && j >= 0 && j < VERTICAL_GRID_SIZE - 1) {
                        grid.SetSquare(i, j, FALLING);
                    }
                }
            }
        }

        return true;
        }
    }
    

    

    return false;
}

void Game::UpdateGame() {
    if (!gameover) {

        if (IsKeyPressed('P')) pause = !pause;

        if (!pause) {
            if (((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                (GetMousePosition().x >= vol.GetPosition().x && GetMousePosition().x <= vol.GetPosition().x + SQUARE_SIZE &&
                    GetMousePosition().y >= vol.GetPosition().y && GetMousePosition().y <= vol.GetPosition().y + SQUARE_SIZE)) ||
                    IsKeyPressed(KEY_M))) {

                        vol.Update();
                        s.PlaySoundN(CLICK);  
            }
            

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

                    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
                        s.PlaySoundN(CLICK);
                        lateralMovementCounter = LATERAL_SPEED;
                    }
                    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_E) || IsKeyPressed(KEY_Q)) {
                        s.PlaySoundN(ROTATE);
                        turnMovementCounter = TURNING_SPEED;
                    }
                    if (IsKeyDown(KEY_S) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER))
                    {   
                        score += 1; 
                        gravityMovementCounter += gravitySpeed;
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
                    if (deletedLines > 0) s.PlaySoundN(LINE_CLEAR);
                    fadeLineCounter = 0;
                    lineDeleting = false;

                    totalLinesCleared += deletedLines;

                    int baseScore = 0;
                    switch (deletedLines) {
                        case 1: baseScore = 40; break;  
                        case 2: baseScore = 100; break; 
                        case 3: baseScore = 300; break; 
                        case 4: baseScore = 1200; break;
                        default: break;
                    }
                    score += baseScore * level;

                    int newLevel = (totalLinesCleared / 10) + 1;
                    if (newLevel > level) {
                        level = newLevel;
                        // Ví dụ: level 1: speed 48, level 2: 43, ..., level 10: 5, level 29+: 1
                        gravitySpeed = std::max(1, 48 - (level - 1) * 5);
                    }
                    
                    fadeLineCounter = 0;
                    lineDeleting = false;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            if (score > highScore) {
                highScore = score;
                SaveHighscore();
            }
            Reset();

        }
    }
}

void Game::DrawGame() {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameover) {

            vol.Draw();

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
            DrawText(TextFormat("SCORE: %06i", score ), controller, offset.y + 10, 14, GRAY);
            DrawText(TextFormat("HIGHEST: %06i", highScore), controller, offset.y + 30, 14, GRAY);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        
        }
        else {
            DrawText(TextFormat("SCORE: %06i", score), screenWidth/2 - MeasureText(TextFormat("HIGH SCORE: %06i", highScore), 20)/2, screenHeight/2 - 80, 20, GRAY);
            DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, screenHeight/2 - 50, 20, GRAY);
        }
    EndDrawing();
}

void Game::UpdateDrawGame() {
    UpdateGame();
    DrawGame();
}

void Game::LoadHighscore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    else {
        highScore = 0;
    }
}

void Game::SaveHighscore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}