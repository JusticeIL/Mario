#pragma once
#include <list>
#include "Level.h"

using namespace std;

class ScreenLoader {

	bool isFileNameValid(string fileName) { return fileName.size() == 15 && fileName.substr(0, 5) == "dkong" && fileName.substr(8) == ".screen" && isdigit(fileName[6]) && isdigit(fileName[7]); }

public:
	list<string> getScreenFileNames();
	Level* TryLoadLevel(string fileName);
};