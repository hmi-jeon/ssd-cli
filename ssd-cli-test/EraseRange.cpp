#pragma once
#include "ICommand.cpp"
#include <vector>

class EraseRange : public ICommand {
public:
	EraseRange(vector<string> args) {
		this->name = "ERASE_RANGE";
		this->args = args;
	};

	virtual bool execute() override {
		if (!checkValidArguments())
			return false;

		string lbaRange = to_string(stoi(args[2]) - stoi(args[1]));
		string command = APP_NAME + " " + "E" + " " + args[1] + " " + lbaRange;
		system(command.c_str());

		return true;
	}
private:
	virtual bool checkValidArguments() override {
		if (args.size() != 3) return false;

		if (!_isValidLba(args[1]) || !_isValidLba(args[2])) {
			return false;
		}

		if (!_isValidEraseRangeSize(stoi(args[1]), stoi(args[2]))) {
			return false;
		}

		return true;
	}

	bool _isValidEraseRangeSize(const int startLba, const int endLba) {
		if (endLba <= startLba) return false;

		int size = endLba - startLba;
		return (startLba + size <= MAX_LBA);
	}
};