#pragma once
#include <vector>
#include <string>
#include "Logger.hpp"

using namespace std;

class ICommand {

public:
	virtual bool execute() = 0;
	virtual bool checkValidArguments() = 0;

protected:
	bool _isValidLba(const string lba) {
		for (char c : lba) {
			if (!std::isdigit(c)) {
				return false;
			}
		}
		int lbaDigit = stoi(lba);
		return (lbaDigit >= 0 && lbaDigit < 100);
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

	string name;
	vector<string> args;
	Logger& logger = Logger::getInstance();
	const string APP_NAME = "ssd-cli.exe";
	static constexpr int MAX_LBA = 100;
};
