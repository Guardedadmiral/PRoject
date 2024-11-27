#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

const int width = 30;
const int height = 30   ;

int planeX, planeY;
int missile1X, missile1Y;
int missile2X, missile2Y;
int score = 0;
bool gameOver = false;
float missileSpeed = 1.2f;
int gameSpeed = 150;
bool hardMode = false;
char planeSymbol = '>';
string playerName;

void Setup();
void Draw();
void Input();
void Logic();
void Menu();
void SaveScore(const string& name, int score);
void ViewScores();
void SetColor(int color);
void EndGame();

void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Setup() {
    planeX = width / 2;
    planeY = height / 2;
    missile1X = rand() % width;
    missile1Y = rand() % height;
    if (hardMode) {
        missile2X = rand() % width;
        missile2Y = rand() % height;
    }
    score = 0;
    gameOver = false;
    planeSymbol = '>';
}

void Draw() {
    system("cls");
    SetColor(9);
    for (int i = 0; i < width + 2; ++i) cout << "l";
    cout << endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width + 2; ++x) {
            if (x == 0 || x == width + 1) {
                SetColor(9);
                cout << "l";
            } else if (x - 1 == planeX && y == planeY) {
                SetColor(10);
                cout << planeSymbol;
            } else if (x - 1 == missile1X && y == missile1Y) {
                SetColor(12);
                cout << "!";
            } else if (hardMode && x - 1 == missile2X && y == missile2Y) {
                SetColor(14);
                cout << "!";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }

    SetColor(9);
    for (int i = 0; i < width + 2; ++i) cout << "l";
    cout << endl;

    SetColor(7);
    cout << "Player: " << playerName << "  Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': if (planeY > 0) planeY--; planeSymbol = '-'; break;
            case 's': if (planeY < height - 1) planeY++; planeSymbol = '-'; break;
            case 'a': if (planeX > 0) planeX--; planeSymbol = '<'; break;
            case 'd': if (planeX < width - 1) planeX++; planeSymbol = '>'; break;
        }
    }
}

void MoveMissile(int& missileX, int& missileY) {
    int dx = planeX - missileX;
    int dy = planeY - missileY;
    float dist = sqrt(dx * dx + dy * dy);
    missileX += (int)(missileSpeed * (dx / dist));
    missileY += (int)(missileSpeed * (dy / dist));
    if (missileX < 0 || missileX >= width || missileY < 0 || missileY >= height) {
        missileX = rand() % width;
        missileY = rand() % height;
    }
}

void Logic() {
    MoveMissile(missile1X, missile1Y);
    if (hardMode) MoveMissile(missile2X, missile2Y);
    if ((missile1X == planeX && missile1Y == planeY) || (hardMode && missile2X == planeX && missile2Y == planeY)) {
        gameOver = true;
    }
    if (planeX < 0 || planeX >= width || planeY < 0 || planeY >= height) gameOver = true;
    score++;
}

void SaveScore(const string& name, int score) {
    ofstream outFile("highscore.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Player: " << name << " | Score: " << score << endl;
        outFile.close();
    }
}

void ViewScores() {
    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        system("cls");
        string line;
        cout << "==== Saved Scores ====" << endl;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    } else {
        cout << "No scores found or error opening file." << endl;
    }
    system("pause");
}

void EndGame() {
    cout << "Game Over! Final Score: " << score << endl;
    SaveScore(playerName, score);
    cout << "Your score has been saved." << endl;
    system("pause");
}

void Menu() {
    int choice;
    bool running = true;
    while (running) {
        system("cls");
        cout << "==== Plane and Missile Game ====\n";
        cout << "1. Start Game (Easy)\n";
        cout << "2. Start Game (Hard)\n";
        cout << "3. View Scores\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter your name: ";
                cin >> playerName;
                hardMode = false;
                Setup();
                while (!gameOver) {
                    Draw();
                    Input();
                    Logic();
                    Sleep(gameSpeed);
                }
                EndGame();
                break;
            case 2:
                cout << "Enter your name: ";
                cin >> playerName;
                hardMode = true;
                Setup();
                while (!gameOver) {
                    Draw();
                    Input();
                    Logic();
                    Sleep(gameSpeed);
                }
                EndGame();
                break;
            case 3:
                ViewScores();
                break;
            case 4:
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again.\n";
                system("pause");
        }
    }
}

int main() {
    Menu();
    return 0;
}
