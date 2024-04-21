#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

#define MAX_SIZE 5
#define LOOP_MAX_SIZE 30

using namespace std;

int main(int argc, char* argv[]) {
	string fileName = "ssd.exe";
	string oldTtestData = "0xAAAABBBB";
	string newTtestData = "0x12345678";
	string command = "";
	for (int i = 0; i < LOOP_MAX_SIZE; i++) {
		for (int lba = 0; lba <= MAX_SIZE; lba++) {
			command = fileName + " " + "W" + " " + to_string(lba) + " " + oldTtestData;
			system(command.c_str());
		}
	}
	for (int lba = 0; lba <= MAX_SIZE; lba++) {
		command = fileName + " " + "W" + " " + to_string(lba) + " " + newTtestData;
		system(command.c_str());
	}
	for (int lba = 0; lba <= MAX_SIZE; lba++) {
		command = fileName + " " + "R" + " " + to_string(lba);
		system(command.c_str());

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open())
			resultFile >> data;

		if (data.compare(newTtestData) != 0)
			return 1;
	}

	return 0;
}