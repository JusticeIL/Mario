#include <iostream>
#include "Board.h"

using namespace std;

// This function prints the current state of the board
void Board::print() const {
    for (const string& line : gameBoard)
        cout << line << endl;
}