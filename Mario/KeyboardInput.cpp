#include "KeyboardInput.h"
#include <cctype>
#include <conio.h>

// This function returns the key the user pressed as a lowercase char, or a null char if no key was pressed
char KeyboardInput::getInput(int currentTick) {
	if (_kbhit())
		return static_cast<char>(tolower(_getch()));

	return '\0';
}