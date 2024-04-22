#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "inand.h"

using namespace std;

struct WriteBuffer {
	string data[100];
	int dirty[100];
	int cnt;
};

class ICommand {
public:
	virtual void execute(vector<string> cmdString, INAND* nand_, WriteBuffer& buffer) = 0;

protected:
	bool _isValidLba(const int lba) {
		return (lba >= 0 && lba < 100);
	}

	bool _isValidValue(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		for (const char& c : value.substr(2)) {
			if (!isxdigit(c)) {
				return false;
			}
		}
		return true;
	}

	bool isNumber(const string& str) {
		for (char const& c : str) {
			if (std::isdigit(c) == 0) return false;
		}
		return true;
	}

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}
};