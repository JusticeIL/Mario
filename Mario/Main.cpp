#include "HelperFunc.h"
#include "Board.h"
#include "Menu.h"

int main(int argc, char* argv[]) {

	ShowConsoleCursor(false);
	Board gameBoard;
	Menu gameMenu(gameBoard);
	gameMenu.run();
	
	return 0;
}