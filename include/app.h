#pragma once

#include <string>
#include <vector>

using strvec = std::vector <std::string>;

enum fileactionChoice { ovrwrite = 1, append = 2, edit = 3 };
enum editChoice { insertbelow = 1, replace = 2, dlete = 3 };

class App {
public:
	void run();
private:
	void openFileFlow(const std::string& fileName);
	std::string getFileName();
};

void printStr(const std::string& str);
void listExisting();
void startFunc();
void timestampCreate(std::string& fileName);
void printFile(const std::string& fileName);
void inputLines(std::ofstream& textfile);
void overwriteFile(const std::string& fileName, int ow);
void appendFile(const std::string& fileName);
strvec loadFileVec(const std::string& fileName);
void saveVecFile(const strvec& fileVector, const std::string fileName);
void insertBelowLine(strvec& fileVector, const std::string fileName);
void replaceLine(strvec& fileVector, const std::string fileName);
void deleteLine(strvec& fileVector, const std::string fileName);
fileactionChoice fileAction();
editChoice editAction();