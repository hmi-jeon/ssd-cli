#pragma once
#include <string>

using namespace std;

class TestShell {
public:
	int cnt{ 0 };
	string line{};

	virtual void write(int LBA, string data) {

	};

	virtual void read(int pos) {
	};

	virtual void exit() {
	};
};