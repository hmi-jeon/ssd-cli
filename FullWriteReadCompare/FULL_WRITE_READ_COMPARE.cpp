#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "../ssd-cli-test/Logger.hpp"

#define MAX_SIZE 100

using namespace std;

const string APP_NAME = "ssd.exe";
const string RESULT_FILE = "result.txt";

void write(const int lba, const string data) {
	string command = APP_NAME + " " + "W" + " " + to_string(lba) + " " + data;
	system(command.c_str());
}

void read(const int lba) {
	string command = APP_NAME + " " + "R" + " " + to_string(lba);
	system(command.c_str());
}

string getResultFile() {
	ifstream resultFile;
	string data = "";
	resultFile.open(RESULT_FILE);
	if (resultFile.is_open()) resultFile >> data;
	return data;
}

int main(int argc, char* argv[]) {
	const string testData = "0x12345678";
	Logger& logger = Logger::getInstance();
	if (argc > 1) logger.setLoggerMode(RUNNER_MODE);
	logger.print("[START] FULL_WRITE_READ_COMPARE");

	//fullwrite
	for (int lba = 0; lba < MAX_SIZE; lba++)
		write(lba, testData);

	//fullread & prove
	for (int lba = 0; lba < MAX_SIZE; lba++) {
		read(lba);
		if (getResultFile().compare(testData) != 0) return EXIT_FAILURE;
	}

	logger.print("[END] FULL_WRITE_READ_COMPARE");
	return EXIT_SUCCESS;
}