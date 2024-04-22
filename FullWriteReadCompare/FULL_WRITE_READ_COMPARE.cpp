#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "../ssd-cli-test/Logger.hpp"

#define MAX_SIZE 100

using namespace std;

int main(int argc, char* argv[]) {
	string fileName = "ssd.exe";
	string testData = "0x12345678";
	string command = "";
	Logger& logger = Logger::getInstance();
	logger.print("[START] FULL_WRITE_READ_COMPARE");

	for (int lba = 0; lba < MAX_SIZE; lba++) {
		command = fileName + " " + "W" + " " + to_string(lba) + " " + testData;
		system(command.c_str());
	}

	for (int lba = 0; lba < MAX_SIZE; lba++) {
		command = fileName + " " + "R" + " " + to_string(lba);
		system(command.c_str());
		
		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open())
			resultFile >> data;

		if (data.compare(testData) != 0)
			return 1;
	}

	logger.print("[END] FULL_WRITE_READ_COMPARE");
	return 0;
}