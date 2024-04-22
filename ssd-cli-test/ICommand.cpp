#pragma once
#include <vector>
#include <string>
#include "Logger.hpp"

using namespace std;

class ICommand {

public:
	virtual bool execute(vector<string> args) = 0;
	virtual bool checkValidArguments(vector<string> args) = 0;
	string getCommandName() const {
		return name;
	}

	void printHelp() const {
		cout << this->name << " : " << this->description << endl;
	}

protected:
	bool _isValidLba(const string lba) {
		for (const char& c : lba) {
			if (!std::isdigit(c)) {
				logger.print("LBA input is not digit.");
				return false;
			}
		}
		int lbaDigit = stoi(lba);
		if (lbaDigit >= 0 && lbaDigit < MAX_LBA)
			return true;
		else {
			logger.print("LBA is Out of range.");
			return false;
		}
	}

	bool _isValidValue(const string value) {
		if ((value.size() != 10) || (value.substr(0, 2) != "0x")) {
			logger.print("Address is incorrect format!");
			return false;
		}

		for (const char& c : value.substr(2)) {
			if (!isxdigit(c)) {
				logger.print("Address is not hex value!");
				return false;
			}
		}
		return true;
	}

	bool isNumber(const string& str) {
		for (const char& c : str) {
			if (std::isdigit(c) == 0) return false;
		}
		return true;
	}

	string name;
	string description;
	Logger& logger = Logger::getInstance();
	const string APP_NAME = "ssd.exe";
	static constexpr int MAX_LBA = 100;
};
