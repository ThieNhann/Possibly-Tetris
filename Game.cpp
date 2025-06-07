#include "Game.h"

Game::Game() : volumeButton(Vector2{screenWidth - 2 * BUTTON_SIZE, 10 + 2 * BUTTON_SIZE}) {

    LoadHighscore();
    pauseButton = std::make_unique<Button>(Vector2{screenWidth - 2 * BUTTON_SIZE, BUTTON_SIZE}, BUTTON_SIZE, BUTTON_SIZE, LoadTexture("resources/image/pause.png"),
                                        [&]() {
                                            pause = !pause;
                                        });
    rotateLeftButton = std::make_unique<Button>(Vector2{10 + BIG_BUTTON_SIZE, screenHeight - 5 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/rotateLeft.png"),
                                        [&]() {
                                            requestedRotation = COUNTERCLOCKWISE;
                                            turnMovementCounter = TURNING_SPEED;
                                            s.PlaySoundN(ROTATE);
                                        });
    rotate180Button = std::make_unique<Button>(Vector2{30 + BIG_BUTTON_SIZE * 2, screenHeight - 5 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/rotate180.png"),
                                        [&]() {
                                            requestedRotation = R180;
                                            turnMovementCounter = TURNING_SPEED;
                                            s.PlaySoundN(ROTATE);
                                        });
    rotateRightButton = std::make_unique<Button>(Vector2{50 + BIG_BUTTON_SIZE * 3, screenHeight - 5 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/rotateRight.png"),
                                        [&]() {
                                            requestedRotation = CLOCKWISE;
                                            turnMovementCounter = TURNING_SPEED;
                                            s.PlaySoundN(ROTATE);
                                        });
    moveLeftButton = std::make_unique<Button>(Vector2{10 + BIG_BUTTON_SIZE, screenHeight + 20 - 4 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/moveLeft.png"),
                                        [&]() {
                                            requestedMoveDirection = LEFT;
                                            s.PlaySoundN(CLICK);
                                            lateralMovementCounter = LATERAL_SPEED;
                                        });      
    moveRightButton = std::make_unique<Button>(Vector2{50 + BIG_BUTTON_SIZE * 3, screenHeight + 20 - 4 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/moveRight.png"),
                                        [&]() {
                                            requestedMoveDirection = RIGHT;
                                            s.PlaySoundN(CLICK);
                                            lateralMovementCounter = LATERAL_SPEED;
                                        });
    fastDropButton = std::make_unique<Button>(Vector2{screenWidth - BIG_BUTTON_SIZE * 2, screenHeight - 5 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/fastDrop.png"),
                                        [&]() {
                                            if (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER) {
                                                score += 3; 
                                                gravityMovementCounter += fastFallSpeed;
                                            }
                                        });           
    hardDropButton = std::make_unique<Button>(Vector2{screenWidth - BIG_BUTTON_SIZE * 2, screenHeight + 20 - 4 * BIG_BUTTON_SIZE}, BIG_BUTTON_SIZE, BIG_BUTTON_SIZE, LoadTexture("resources/image/hardDrop.png"),
                                        [&]() {
                                            Harddrop();
                                            CheckCompletedLine();
                                            gravityMovementCounter = 0;
                                        });                                                                
    Reset();
}

void Game::Reset() {
    grid = Grid();
    incomingPiece = Piece();
    level = 1;
    score = 0;
    totalLinesCleared = 0;

    fastFallSpeed = 45;
    gravitySpeed = 45;
    activePiece.SetPosition({0, 0});

    pause = false;

    requestedRotation = NONE;
    begin = true;
    pieceActivating = false;
    lineDeleting = false;
    gameover = false;

    gravityMovementCounter = 0;
    lateralMovementCounter = 0;
    turnMovementCounter = 0;
    fastFallMovementCounter = 0;
    fadeLineCounter = 0;
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

    if (!requestedMoveDirection) return false;

    bool collision = false;

    switch (requestedMoveDirection) {
        case LEFT: {
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

            break;
        }
        case RIGHT: {
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
            
            break;
        }
        default: return false;
    }

    return collision;
}

bool Game::UpdateTurningMovement() {
    if (requestedRotation == NONE) return false;

    Piece rotatedMatrix = activePiece;

    switch (requestedRotation) {
        case CLOCKWISE:
            rotatedMatrix.RotateClockwise();
            break;
        case COUNTERCLOCKWISE:
            rotatedMatrix.RotateCounterclockwise();
            break;
        case R180:
            rotatedMatrix.Rotate180();
            break;
        default:
            return false;
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

    return false;
}

void Game::UpdateGame() {
    if (!gameover) {

        if (IsKeyPressed('P')) pause = !pause;

        if (!pause) {
            
            pauseButton->Update();
            volumeButton.Update();

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
                    
                    rotateLeftButton->Update();
                    rotate180Button->Update();
                    rotateRightButton->Update();
                    moveLeftButton->Update();
                    moveRightButton->Update();
                    fastDropButton->Update();
                    hardDropButton->Update();

                    if (IsKeyPressed(KEY_SPACE)) {
                        Harddrop();
                        CheckCompletedLine();
                        gravityMovementCounter = 0;
                    }
                    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
                        s.PlaySoundN(CLICK);
                        lateralMovementCounter = LATERAL_SPEED;
                        if (IsKeyPressed(KEY_A)) requestedMoveDirection = LEFT;
                        else if (IsKeyPressed(KEY_D)) requestedMoveDirection = RIGHT;
                    }
                    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_E) || IsKeyPressed(KEY_Q)) {
                        s.PlaySoundN(ROTATE);
                        turnMovementCounter = TURNING_SPEED;
                        if (IsKeyPressed(KEY_W)) requestedRotation = R180;
                        else if (IsKeyPressed(KEY_Q)) requestedRotation = COUNTERCLOCKWISE;
                        else if (IsKeyPressed(KEY_E)) requestedRotation = CLOCKWISE;
                    }
                    if (IsKeyDown(KEY_S) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER))
                    {   
                        score += 3; 
                        gravityMovementCounter += gravitySpeed;
                    }

                    if (gravityMovementCounter >= gravitySpeed) {
                        UpdateFalling();
                        CheckCompletedLine();
                        gravityMovementCounter = 0;
                    }

                    if (lateralMovementCounter >= LATERAL_SPEED)
                    {
                        if (!UpdateSideMovement()) {
                            requestedMoveDirection = NONEm;
                            lateralMovementCounter = 0;
                        }
                    }

                    if (turnMovementCounter >= TURNING_SPEED)
                    {

                        if (UpdateTurningMovement()) {
                            turnMovementCounter = 0;
                            requestedRotation = NONE;
                        }
                        
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
                        gravitySpeed = std::max(1, 45 - (level - 1) * 3);
                    }
                    
                    fadeLineCounter = 0;
                    lineDeleting = false;
                }
            }
        } else {
            if (CheckCollisionPointRec(GetMousePosition(), Rectangle{0, 0, screenWidth, screenHeight}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                pause = !pause;
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

            pauseButton->Draw();
            volumeButton.Draw();
            rotateLeftButton->Draw();
            rotate180Button->Draw();
            rotateRightButton->Draw();
            moveLeftButton->Draw();
            moveRightButton->Draw();
            fastDropButton->Draw();
            hardDropButton->Draw();


            Vector2 offset;
            offset.x = (screenWidth - (HORIZONTAL_GRID_SIZE * SQUARE_SIZE)) / 2;
            offset.y = (screenHeight - ((VERTICAL_GRID_SIZE - 5) * SQUARE_SIZE)) / 2 - 100;

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

            if (pause) {
                DrawRectangle(0, 0, screenWidth, screenHeight, {200, 200, 200, 128});
                DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
                DrawText("TAP ANYWHERE TO CONTINUE", screenWidth/2 - MeasureText("TAP ANYWHERE TO CONTINUE", 20)/2, screenHeight/2, 20, GRAY);
            }
        
        }
        else {
            DrawText(TextFormat("SCORE: %06i", score), screenWidth/2 - MeasureText(TextFormat("HIGH SCORE: %06i", highScore), 20)/2, screenHeight/2 - 80, 20, GRAY);
            DrawText("PRESS [ENTER] TO PLAY", screenWidth/2 - MeasureText("PRESS [ENTER] TO PLAY", 20)/2, screenHeight/2 - 50, 20, GRAY);
        }
    EndDrawing();
}

void Game::UpdateDrawGame() {
    UpdateGame();
    DrawGame();
}

void Game::Harddrop() {
    int rowDropped = 0;
    
    while (!CheckCollision()) {

        ++rowDropped;
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

    for (int j = VERTICAL_GRID_SIZE - 2; j >= 0; --j) {
            for (int i = 0; i < HORIZONTAL_GRID_SIZE - 1; ++i) {
                if (grid.GetSquare(i, j) == FALLING) {
                    grid.SetSquare(i, j, FULL);
            }
        }
    }

    if (rowDropped > 0) {
        score += rowDropped * 5;
    }

    s.PlaySoundN(PIECE_LANDED);
    pieceActivating = false;
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