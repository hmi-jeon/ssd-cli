#pragma once
#include "ICommand.cpp"
#include <vector>

class Erase : public ICommand {
public:
	Erase(vector<string> args) {
		this->name = "ERASE";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		string command = APP_NAME + " " + "E" + " " + args[1] + " " + args[2];
		system(command.c_str());

		return true;
	}
private:
	virtual bool checkValidArguments() override {
		if (args.size() != 3) return false;

		if (!_isValidLba(args[1]) || !_isValidEraseSize(args[2])) {
			return false;
		}

		return _isMaxEraseSize(stoi(args[1]), stoi(args[2]));
	}

	bool _isValidEraseSize(const string size) {
		for (const char& c : size) {
			if (!std::isdigit(c)) {
				return false;
			}
		}
		int sizeDigit = stoi(size);
		return (sizeDigit > 0 && sizeDigit <= 10);
	}

	bool _isMaxEraseSize(const int lba, const int size) {
		return (lba + size <= MAX_LBA);
	}
};
