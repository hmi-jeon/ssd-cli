#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>

#define MAX_SIZE 100

using namespace std;

class TestShell {
public:
	bool status{ true };
	string line{};
	int adr[100] = { 0 , };


	void write(int lba, string data) {
		string command = "C:\\Users\\User\\Desktop\\ssd-cli\\Sample-Test2\\ssd-cli.exe";
		command = command + " " + "W" + " " + to_string(lba) + " " + data;
		system(command.c_str());
	};

	string read(int lba) {
		// ssd.exe call
		string command = "C:\\Users\\User\\Desktop\\ssd-cli\\Sample-Test2\\ssd-cli.exe";
		command = command + " " + "R" + " " + to_string(lba);
		system(command.c_str());

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open()) {
			resultFile >> data;

			cout << data << endl;
		}
		return data;
	};

	void fullread() {
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			read(lba);
		}
	};

	void exit() {
	};

	void help() {
		cout << "--help--" << endl;
		cout << "--[write]--" << endl;
		cout << "--format: write [LBA] [DATA]--" << endl;
	}

	void fullwrite(string data) {
		for (int i = 0; i < 100; i++) {
			write(i, data);
		}
	}
};