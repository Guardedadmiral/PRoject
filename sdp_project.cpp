#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>   // For file handling
#include <graphics.h> // Graphics library for rendering

using namespace std;

const int width = 1024;  // Increased screen width
const int height = 768; // Increased screen height
int planeX = width / 2, planeY = height / 2;
int missile1X = rand() % width, missile1Y = rand() % height;
int missile2X = rand() % width, missile2Y = rand() % height;
int score = 0;
int highScore = 0;  // Variable to store high score
bool gameOver = false;
bool hardMode = false;

const char* highScoreFile = "highscore.txt";

// Load high score from file
void LoadHighScore() {
    ifstream file(highScoreFile);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

// Save high score to file
void SaveHighScore() {
    ofstream file(highScoreFile);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

// Draw the game objects
void Draw() {
    cleardevice(); // Clear screen

    // Draw plane
    setcolor(WHITE);
    rectangle(planeX, planeY, planeX + 40, planeY + 20); // Plane shape

    // Draw missiles
    setcolor(RED);
    circle(missile1X, missile1Y, 10);
    if (hardMode) circle(missile2X, missile2Y, 10);

    // Draw score
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    outtextxy(10, 10, scoreText);

    delay(20); // Slight delay for smoother rendering
}

// Handle movement
void MoveMissiles() {
    // Missile 1
    int dx1 = planeX - missile1X, dy1 = planeY - missile1Y;
    float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);
    missile1X += (int)(2.5 * (dx1 / dist1));
    missile1Y += (int)(2.5 * (dy1 / dist1));

    // Missile 2 (if in hard mode)
    if (hardMode) {
        int dx2 = planeX - missile2X, dy2 = planeY - missile2Y;
        float dist2 = sqrt(dx2 * dx2 + dy2 * dy2);
        missile2X += (int)(2.5 * (dx2 / dist2));
        missile2Y += (int)(2.5 * (dy2 / dist2));
    }
}

// Detect collision
bool CheckCollision() {
    if (abs(planeX - missile1X) < 20 && abs(planeY - missile1Y) < 20) return true;
    if (hardMode && abs(planeX - missile2X) < 20 && abs(planeY - missile2Y) < 20) return true;
    return false;
}

// Update game state
void Update() {
    MoveMissiles();
    if (CheckCollision()) gameOver = true;
    score++;
}

// Keyboard input for controlling the plane
void Input(char key) {
    switch (key) {
        case 'w': if (planeY > 0) planeY -= 10; break;
        case 's': if (planeY < height - 20) planeY += 10; break;
        case 'a': if (planeX > 0) planeX -= 10; break;
        case 'd': if (planeX < width - 40) planeX += 10; break;
    }
}

// Main game loop
void GameLoop() {
    if (!gameOver) {
        Update();
        Draw();
    } else {
        cleardevice();
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        const char* gameOverMessage = "Game Over!";
        outtextxy(width / 2 - 100, height / 2, (char*)gameOverMessage); // Cast to char*
        char finalScore[50];
        sprintf(finalScore, "Score: %d", score);
        outtextxy(width / 2 - 100, height / 2 + 50, finalScore);

        if (score > highScore) {
            highScore = score;
            SaveHighScore();  // Save the new high score
        }

        char highScoreText[50];
        sprintf(highScoreText, "High Score: %d", highScore);
        outtextxy(width / 2 - 100, height / 2 + 100, highScoreText);

        const char* restartMessage = "Click to Restart or Click on Quit";
        outtextxy(width / 2 - 150, height / 2 + 150, (char*)restartMessage);

        // Wait for user input to restart or quit
        int mouseX, mouseY;
        while (true) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
                // Check if within restart button
                if (mouseX >= width / 2 - 150 && mouseX <= width / 2 + 150 &&
                    mouseY >= height / 2 + 150 && mouseY <= height / 2 + 180) {
                    gameOver = false;
                    score = 0;
                    planeX = width / 2;
                    planeY = height / 2;
                    missile1X = rand() % width;
                    missile1Y = rand() % height;
                    missile2X = rand() % width;
                    missile2Y = rand() % height;
                    break;
                } 
                // Check if within quit button
                else if (mouseX >= width / 2 - 150 && mouseX <= width / 2 + 150 &&
                    mouseY >= height / 2 + 200 && mouseY <= height / 2 + 230) {
                    closegraph();
                    exit(0);
                }
            }
        }
    }
}

// Display the menu in the top left corner
void ShowMenu() {
    cleardevice();
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2); // Medium font size

    // Display the title
    const char* menuTitle = "Plane Apocalypse";
    outtextxy(10, 10, (char*)menuTitle); // Top-left corner

    // Display menu options
    const char* startGameOption = "Start Game";
    outtextxy(10, 40, (char*)startGameOption);

    const char* highScoreOption = "High Score";
    outtextxy(10, 70, (char*)highScoreOption);

    const char* exitOption = "Quit";
    outtextxy(10, 100, (char*)exitOption);

    // Wait for mouse click
    int mouseX, mouseY;
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
            // Check if within Start Game button
            if (mouseX >= 10 && mouseX <= 200 && mouseY >= 40 && mouseY <= 70) {
                gameOver = false;
                score = 0;
                planeX = width / 2;
                planeY = height / 2;
                missile1X = rand() % width;
                missile1Y = rand() % height;
                missile2X = rand() % width;
                missile2Y = rand() % height;
                while (!gameOver) {
                    if (kbhit()) {
                        char key = getch();
                        Input(key);
                    }
                    GameLoop();
                }
                ShowMenu(); // Go back to menu after game ends
            }
            // Check if within High Score button
            else if (mouseX >= 10 && mouseX <= 200 && mouseY >= 70 && mouseY <= 100) {
                cleardevice();
                settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2); // Medium font size
                char highScoreText[50];
                sprintf(highScoreText, "High Score: %d", highScore);
                outtextxy(width / 2 - 100, height / 2, highScoreText);
                const char* backOption = "Click to go back to the menu";
                outtextxy(width / 2 - 150, height / 2 + 50, (char*)backOption);
                while (true) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
                        if (mouseX >= width / 2 - 150 && mouseX <= width / 2 + 150 &&
                            mouseY >= height / 2 + 50 && mouseY <= height / 2 + 80) {
                            ShowMenu();
                            break;
                        }
                    }
                }
            }
            // Check if within Exit button
            else if (mouseX >= 10 && mouseX <= 200 && mouseY >= 100 && mouseY <= 130) {
                closegraph();
                exit(0);
            }
        }
    }
}

int main() {
    // Initialize graphics window
    int gd = DETECT, gm;
    const char* path = "";
    initgraph(&gd, &gm, (char*)path); // Cast to char*

    LoadHighScore();  // Load the high score from file

    ShowMenu();  // Show the main menu

    closegraph();
    return 0;
}
