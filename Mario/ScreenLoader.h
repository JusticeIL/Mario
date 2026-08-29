#pragma once
#include <list>
#include "Level.h"

class ScreenLoader {

	bool isFileNameValid(const std::string& fileName) { return fileName.size() == 15 && fileName.substr(0, 5) == "dkong" && fileName.substr(8) == ".screen" && isdigit(fileName[6]) && isdigit(fileName[7]); }

public:
	std::list<std::string> getScreenFileNames();
	Level* TryLoadLevel(const std::string& fileName);
};