#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "../ssd-cli-test/Logger.hpp"

#define MAX_SIZE 5

using namespace std;

const string APP_NAME = "ssd.exe";
const string RESULT_FILE = "result.txt";

void read(const int lba) {
	string command = APP_NAME + " " + "R" + " " + to_string(lba);
	system(command.c_str());
}

int main(int argc, char* argv[]) {
	Logger& logger = Logger::getInstance();
	if (argc > 1) logger.setLoggerMode(RUNNER_MODE);
	logger.print("[START] REPEAT_READ_TEST");


	for (int lba = 0; lba <= MAX_SIZE; lba++) {
		logger.print("[REPEAT_READ_TEST] This log Refeat during LOOP MAX SIZE for This logger slip and compress test");
		read(lba);
	}
	logger.print("[END] REPEAT_READ_TEST");
	return EXIT_SUCCESS;
}