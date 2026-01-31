#include "app.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <ctime>


namespace fs = std::filesystem;

void App::run() {
	startFunc();
	while (true) {
		clearScreen();
		printHeader();
		std::string fileName = getFileName();
		openFileFlow(fileName);
		waitEnter();
	}
}

std::string App::getFileName() {
	std::string fileName;
	printStr("Enter file name to open:");
	std::getline(std::cin, fileName);
	if (fileName == "") {
		timestampCreate(fileName);
	}
	else if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".txt") {
		fileName += ".txt";
	}
	return fileName;
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void App::openFileFlow(const std::string& fileName) {
	if (fs::exists("notes/" + fileName)) {	// Print existing file contents
		printFile(fileName);
		fileactionChoice choice = fileAction();

		if (choice == ovrwrite) {
			overwriteFile(fileName, 1);
		}

		if (choice == append) {
			appendFile(fileName);
		}

		if (choice == edit) {
			strvec fileVector = loadFileVec(fileName);

			editChoice editChoice = editAction();
			if (editChoice == insertbelow) {
				insertBelowLine(fileVector, fileName);
			}
			if (editChoice == replace) {
				replaceLine(fileVector, fileName);
			}
			if (editChoice == dlete) {
				deleteLine(fileVector, fileName);
			}

			saveVecFile(fileVector, fileName);
			printFile(fileName);
		}
	}
	else {
		printStr(fileName);
		overwriteFile(fileName, 0);
	}
}

void printStr(const std::string& str) {
	std::cout << str << "\n";
}

void listExisting() {	//Print text files in notes directory
	std::vector <std::string> notesList;
	for (const auto& entry : fs::directory_iterator("notes")) {
		std::string textfilename = entry.path().filename().string();
		if (textfilename.size()>=4 && textfilename.substr(textfilename.size() - 4, 4) == ".txt") {
			notesList.push_back(textfilename);
		}
	}
	if (!notesList.empty()){
		for (std::string& name : notesList) {
			printStr(name);
		}
	}
	else { printStr("No text files in directory\n"); }
}

void startFunc() {
	//Create notes directory
	if (!fs::exists("notes")) {
		fs::create_directory("notes");
	}
}

void printHeader() {
	printStr("\tNOTESAPP");
	listExisting();
}

void waitEnter() {
	printStr("Press Enter to CONTINUE");
	std::string _;
	std::getline(std::cin, _);
}

void timestampCreate(std::string& fileName) {	//Timestamp created if name empty
	time_t now = time(0);
	struct tm t;
	localtime_s(&t, &now);
	char buf[32];
	strftime(buf, sizeof(buf), "Note-%H-%M-%d-%m-%Y.txt", &t);
	fileName = buf;
	printStr("Creating file with name:");
	printStr(fileName);
}

void printFile(const std::string& fileName) {
	printStr("\nContents of " + fileName);
	std::ifstream textRead(std::string("notes/") + fileName);	//open file for reading
	std::string fileContents;
	int i = 1;
	while (std::getline(textRead, fileContents)) {
		printStr(std::to_string(i) + " " + fileContents);
		i++;
	}
}

fileactionChoice fileAction() {
	printStr("Choose action \n1 Overwrite\n2 Append\n3 Edit");
	std::string temp;
	std::getline(std::cin, temp);
	int tempnum = std::stoi(temp);
	fileactionChoice choice = static_cast<fileactionChoice>(tempnum);
	return choice;
}

editChoice editAction() {
	printStr("Choose action \n1 Insert below line\n2 Replace line\n3 Delete line");
	std::string temp;
	std::getline(std::cin, temp);
	int tempnum = std::stoi(temp);
	editChoice choice = static_cast<editChoice>(tempnum);
	return choice;
}

void inputLines(std::ofstream& textfile) {
	std::string lineBuffer;
	do {
		std::getline(std::cin, lineBuffer);
		if (lineBuffer.size() >= 2 && lineBuffer.substr(lineBuffer.size() - 2, 2) == "\\e") {	//Check if last 2 characters are \e
			textfile << lineBuffer.substr(0, lineBuffer.size() - 2) << "\n";	//remove \e from line
			break;
		}
		textfile << lineBuffer + "\n";	//add line to textfile
	} while (true);
}

void overwriteFile(const std::string& fileName, int ow) {
	std::ofstream textfile(std::string("notes/") + fileName);	//Automatically makes and .open()s the file
	if (ow == 1) {
		printStr("^ Chose to overwrite file");
	}
	else {
		printStr("^ Chose to write new file");
	}
	printStr("Enter Contents of " + fileName);
	//Input buffer
	inputLines(textfile);
	//textfile.close();	Closes automatically when returning so not needed
}

void appendFile(const std::string& fileName) {
	std::ofstream textfile(std::string("notes/") + fileName, std::ios::app);	//Automatically makes and .open()s the file
	printStr("^ Chose to append file");
	printFile(fileName);
	printStr("...");
	//Input buffer
	inputLines(textfile);
	//textfile.close();	Closes automatically when returning so not needed
}

strvec loadFileVec(const std::string& fileName) {
	std::ifstream textRead(std::string("notes/") + fileName);
	strvec fileContents;
	std::string lineAdd;
	while (getline(textRead, lineAdd)) {
		fileContents.push_back(lineAdd);
	}
	return fileContents;
}

void saveVecFile(const strvec& fileVector, const std::string fileName) {
	std::ofstream writeText("notes/" + fileName);
	for (const std::string& line : fileVector) {
		writeText << line << "\n";
	}
}

void insertBelowLine(strvec& fileVector, const std::string fileName) {
	std::string temp = "";
	printFile(fileName);
	printStr("Enter line after which to :");
	std::getline(std::cin, temp);
	int n = std::stoi(temp);
	printStr(fileVector[n - 1]);
	printStr("Enter new line:");
	std::getline(std::cin, temp);
	fileVector.insert(fileVector.begin() + n, temp);
}

void replaceLine(strvec& fileVector, const std::string fileName) {
	std::string temp = "";
	printFile(fileName);
	printStr("Enter line to replace :");
	std::getline(std::cin, temp);
	int n = std::stoi(temp);
	printStr(fileVector[n - 1]);
	printStr("Enter new line:");
	std::getline(std::cin, temp);
	fileVector[n - 1] = temp;
}

void deleteLine(strvec& fileVector, const std::string fileName) {
	std::string temp = "";
	printFile(fileName);
	printStr("Enter line after which to :");
	std::getline(std::cin, temp);
	int n = std::stoi(temp);
	printStr(fileVector[n - 1]);
	fileVector.erase(fileVector.begin() + n - 1);
}