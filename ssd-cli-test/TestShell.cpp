#pragma once
#include <string>
#include <process.h>

using namespace std;

class TestShell {
public:
	int cnt{ 0 };
	string line{};

	void write(int lba, string data) {
		string command = "ssd.exe W" + lba + ' ' + data;
		system(command.c_str());
	};

	void read(int lba) {
	};

	void exit() {
	};
};