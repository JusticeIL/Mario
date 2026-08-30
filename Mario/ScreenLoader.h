#pragma once
#include <cctype>
#include <list>
#include <string>
#include "Level.h"

class ScreenLoader {
	// Constant
	static constexpr size_t FILENAME_LENGTH = 15;

	// File name validation
	bool isFileNameValid(const std::string& fileName) const { return fileName.size() == FILENAME_LENGTH && fileName.substr(0, 5) == "dkong" && fileName.substr(8) == ".screen"
		&& fileName[5] == '_'
		&& isdigit(static_cast<unsigned char>(fileName[6]))
		&& isdigit(static_cast<unsigned char>(fileName[7]));
	}

public:
	// Screen file management
	std::list<std::string> getScreenFileNames();
	Level* TryLoadLevel(const std::string& fileName);
};