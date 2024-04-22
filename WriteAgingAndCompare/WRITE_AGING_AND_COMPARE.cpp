#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define MAX_SIZE 5
#define LOOP_MAX_SIZE 30

using namespace std;

const string APP_NAME = "ssd.exe";
const string RESULT_FILE = "result.txt";

void write(int lba, string data) {
	string command = APP_NAME + " " + "W" + " " + to_string(lba) + " " + data;
	system(command.c_str());
}

void read(int lba) {
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
	const string oldTestData = "0xAAAABBBB";
	const string newTestData = "0x12345678";
	string command = "";

	for (int i = 0; i < LOOP_MAX_SIZE; i++) {
		for (int lba = 0; lba <= MAX_SIZE; lba++) {
			write(lba, oldTestData);
		}
	}

	for (int lba = 0; lba <= MAX_SIZE; lba++) {
		write(lba, newTestData);
	}

	for (int lba = 0; lba <= MAX_SIZE; lba++) {
		read(lba);
		if (getResultFile().compare(newTestData) != 0) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}