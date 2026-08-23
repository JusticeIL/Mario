#include "KeyboardInput.h"

#include <cctype>
#include <conio.h>

char KeyboardInput::getInput(int currentTick) {
	if (_kbhit())
		return static_cast<char>(tolower(_getch()));

	return '\0';
}