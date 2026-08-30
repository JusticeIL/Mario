#include <iostream>
#include <windows.h>
#include "HelperFunc.h"

using std::string;

// This function moves the cursor to a specified position in the console window
void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord; // Create a COORD structure 
    coord.X = x; // Set the X coordinate 
    coord.Y = y; // Set the Y coordinate 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}

// This function controls the visibility of the console cursor
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

// This function clears the screen
void clearScr() {
    system("cls");
}

// This function receives command line count and arguments, and turns on the game modes given in the command line 
void chooseGameMode(int argc, char* argv[], bool& isLoad, bool& isSave, bool& isSilent) {
    for (int i = 1; i < argc; ++i) {
        string command = argv[i];

        if (command == "-load") // Case: load
            isLoad = true;
        else if (command == "-silent") // Case: silent
            isSilent = true;
        else if (command == "-save" && !isLoad) // Case: save
            isSave = true;
    }

    if (isLoad && isSave) // If load is on, turn off save
        isSave = false;
}

// This function receives filename, and returns it without its extension
string getBaseName(const string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos)
        return filename.substr(0, dotPos);

    return filename;
}