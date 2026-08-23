#pragma once

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void clearScr();
void chooseGameMode(int argc, char* argv[], bool& isLoad, bool& isSave, bool& isSilent);
std::string getBaseName(const std::string& filename);