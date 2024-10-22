#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <conio.h>

void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

struct SnakeCell {

    // Attributes
    int x;
    int y;
    SnakeCell* NextCell;
    SnakeCell* PreviousCell;

    // Constructors
    SnakeCell(): x(0), y(0), NextCell(nullptr), PreviousCell(nullptr) {}
    SnakeCell(int i, int j, SnakeCell* Next, SnakeCell* Prev): x(i), y(j), NextCell(Next), PreviousCell(Prev) {}

};

class SnakeGame {

private:
    // Attributes
        // Snake
    SnakeCell* SnakeHead;
    SnakeCell* SnakeTail;
    int SnakeSize;
    char SnakeDirection;

        // Plateau
    int Height = 25;
    int Width = 25;
    int SnakeFood[2];
    char** CurrentFrameBuffer;
    char** NextFrameBuffer;

public:
    // Main constructor
    SnakeGame(){
        CurrentFrameBuffer = new char*[25];
        NextFrameBuffer = new char*[25];

        for (int i = 0; i < 25; i ++){

            CurrentFrameBuffer[i] = new char[25];
            NextFrameBuffer[i] = new char[25];

            for (int j = 0; j < 25; j ++){

                CurrentFrameBuffer[i][j] = ' ';
                NextFrameBuffer[i][j] = ' ';
            }
        }

        SnakeFood[0] = 10; SnakeFood[1] = 10;
        SnakeSize = 3;
        SnakeDirection = 'd';

        SnakeCell* C0 = new SnakeCell;
        SnakeCell* C1 = new SnakeCell;
        SnakeCell* C2 = new SnakeCell;

        *C0 = SnakeCell(0, 0, nullptr, C1);
        *C1 = SnakeCell(0, 1, C0, C2);
        *C2 = SnakeCell(0, 2, C1, nullptr);

        SnakeHead = C2;
        SnakeTail = C0;

        NextFrameBuffer[0][0] = '+';
        NextFrameBuffer[0][1] = '+';
        NextFrameBuffer[0][2] = '+';
        NextFrameBuffer[10][10] = 'O';

        CurrentFrameBuffer[0][0] = '+';
        CurrentFrameBuffer[0][1] = '+';
        CurrentFrameBuffer[0][2] = '+';
        CurrentFrameBuffer[10][10] = 'O';

    }

    ~SnakeGame(){
        SnakeCell* Iterateur = SnakeHead;
        while (Iterateur != nullptr){
            SnakeCell* Ck = Iterateur;
            Iterateur = Iterateur->NextCell;
            delete Ck;
        }
    }

    // Main class related functions
    void NewCell();
    void UpdateSnake(char, bool&);
    void UpdateFrame();

    bool TriggerGameOver();
    bool TriggerAddCell();

    void Play();
};

void SnakeGame::NewCell(){

    // Adds a new cell to the snake

    SnakeCell* Cell = new SnakeCell;

    int x0 = SnakeTail->x;
    int y0 = SnakeTail->y;
    SnakeTail->NextCell = Cell;

    *Cell = SnakeCell(x0, y0, nullptr, SnakeTail);
    SnakeTail = Cell;
}

void SnakeGame::UpdateSnake(char UserInput, bool& GameOn){

    // Updates the position of the snake depending on the input from user.
    // Depending whether it is game ending move, modifies a bool "GameOn".

    SnakeCell* Iterateur = SnakeTail;

    while (Iterateur->PreviousCell != nullptr){
        Iterateur->x = Iterateur->PreviousCell->x;
        Iterateur->y = Iterateur->PreviousCell->y;
        Iterateur = Iterateur->PreviousCell;
    }

    switch(UserInput){
        case 'z':{
            if (SnakeDirection == 's'){
                SnakeHead->x = (SnakeHead->x + 1) % 25;
            }
            else {
                SnakeHead->x = (SnakeHead->x - 1) + 24 * (SnakeHead->x == 0);
                SnakeDirection = 'z';
            }
            break;
        }

        case 's':{
            if (SnakeDirection == 'z'){
                SnakeHead->x = (SnakeHead->x - 1) + 24 * (SnakeHead->x == 0);
            }
            else {
                SnakeHead->x = (SnakeHead->x + 1) % 25;
                SnakeDirection = 's';
            }
            break;
        }

        case 'd':{
            if (SnakeDirection == 'q'){
                SnakeHead->y = (SnakeHead->y - 1) + 24 * (SnakeHead->y == 0);
            }
            else {
                SnakeHead->y = (SnakeHead->y + 1) % 25;
                SnakeDirection = 'd';
            }
            break;
        }

        case 'q':{
            if (SnakeDirection == 'd'){
                SnakeHead->y = (SnakeHead->y + 1) % 25;
            }
            else {
                SnakeHead->y = (SnakeHead->y - 1) + 24 * (SnakeHead->y == 0);
                SnakeDirection = 'q';
            }
            break;
        }
    }

    if (TriggerAddCell()){
        NewCell();
        SnakeFood[0] = SnakeTail->x;
        SnakeFood[1] = SnakeTail->y;
        SnakeSize ++;
    }

    if (TriggerGameOver()){
        GameOn = false;
    }
}

void SnakeGame::UpdateFrame(){

    // Given the class attributes of Plateau and Snake, builds and prints NewFrame, stores it in CurrentFrame.

    SnakeCell* Iterateur = SnakeHead;
    for (int i = 0; i < 25; i ++){
        for (int j = 0; j <25; j ++){
            NextFrameBuffer[i][j] = ' ';
        }
    }

    NextFrameBuffer[SnakeFood[0]][SnakeFood[1]] = 'O';

    while (Iterateur != nullptr){
        NextFrameBuffer[Iterateur->x][Iterateur->y] = '+';
        Iterateur = Iterateur->NextCell;
    }

    for (int i = 0; i < 25; i ++){
        for (int j = 0; j < 25; j ++){
            if (CurrentFrameBuffer[i][j] != NextFrameBuffer[i][j]){
                setCursorPosition(j, i);
                std::cout << NextFrameBuffer[i][j];
                CurrentFrameBuffer[i][j] = NextFrameBuffer[i][j];
            }
        }
    }
}

bool SnakeGame::TriggerAddCell(){

    // Checks whether a new cell must be added

    if (SnakeHead->x == SnakeFood[0] && SnakeHead->y == SnakeFood[1]){
        return true;
    }
    else {
        return false;
    }
}

bool SnakeGame::TriggerGameOver(){

    // Checks whether the snake bites itself

    SnakeCell* Iterateur = SnakeTail;
    int x0 = SnakeHead->x;
    int y0 = SnakeHead->y;

    while (Iterateur != SnakeHead){
        if (Iterateur->x == x0 && Iterateur->y == y0){
            break;
        }
        Iterateur = Iterateur->PreviousCell;
    }

    return (Iterateur != SnakeHead);
}

void SnakeGame::Play(){

    // Launches the main game loop - handles player input
    // Default set up : 3 sized snake going to the right

    int ReactionSpan = 50;
    char PlayerInput = 'd';
    bool GameOn = true;

    for (int i = 0; i < 25; i ++){
        for (int j = 0; j < 25; j ++){
            std::cout << CurrentFrameBuffer[i][j];
        }
        std::cout << '\n';
    }

    while(GameOn){
        UpdateFrame();
        int SnakeSpeed = 15;
        int ElapsedTime = 0;

        while (_kbhit() == false && ElapsedTime != SnakeSpeed){
            Sleep(1);
            ElapsedTime ++;
        }

        if (ElapsedTime == SnakeSpeed) {
            PlayerInput = SnakeDirection;
        }
        else{
            PlayerInput = getch();
        }

        UpdateSnake(PlayerInput, GameOn);
    }

    setCursorPosition(0, 25);
    std::cout << "Game Has Stopped - Score : " << SnakeSize << std::endl;
}

int main(){
    ShowConsoleCursor(false);
    SnakeGame Demo;
    Demo.Play();
}
